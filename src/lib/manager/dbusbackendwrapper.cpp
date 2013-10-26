/*
 * Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

/**
 * @file dbusbackendwrapper.cpp
 * @short Implementation of PT2::DBusBackendWrapper
 */

#include "dbusbackendwrapper.h"
#include "manager/abstractbackendwrapper_p.h"

#include <QtCore/QProcess>
#include <QtCore/QStringList>
#include <QtDBus/QDBusConnection>

#include "capabilitiesconstants.h"
#include "debug.h"
#include "dbus/dbushelper.h"
#include "dbus/dbusconstants.h"
#include "dbus/generated/dbusbackendwrapperadaptor.h"

namespace PT2
{

/**
 * @internal
 * @brief Private class for PT2::DBusBackendWrapper
 */
class DBusBackendWrapperPrivate: public AbstractBackendWrapperPrivate
{
    Q_OBJECT
public:
    /**
     * @internal
     * @brief Default constructor
     * @param q Q-pointer
     */
    explicit DBusBackendWrapperPrivate(DBusBackendWrapper *q);
    /**
     * @internal
     * @brief Process
     */
    QProcess *process;
    /**
     * @internal
     * @brief DBus object path
     */
    QString dbusObjectPath;
public Q_SLOTS:
    /**
     * @internal
     * @brief Slot for read standard output
     */
    void slotReadStandardOutput();
    /**
     * @internal
     * @brief Slot for read standard error
     */
    void slotReadStandardError();
    /**
     * @internal
     * @brief Slot for process error
     * @param error error.
     */
    void slotProcessError(QProcess::ProcessError error);
    /**
     * @internal
     * @brief Slot for finished
     * @param code exit code.
     */
    void slotFinished(int code);
private:
    /**
     * @internal
     * @brief Q-pointer
     */
    DBusBackendWrapper * const q_ptr;
    Q_DECLARE_PUBLIC(DBusBackendWrapper)

};

DBusBackendWrapperPrivate::DBusBackendWrapperPrivate(DBusBackendWrapper *q):
    AbstractBackendWrapperPrivate(), q_ptr(q)
{
}

void DBusBackendWrapperPrivate::slotReadStandardOutput()
{
    process->setReadChannel(QProcess::StandardOutput);
    while (!process->atEnd()) {
        debug("backend") << process->readLine().trimmed().constData();
    }
}

void DBusBackendWrapperPrivate::slotReadStandardError()
{
    process->setReadChannel(QProcess::StandardError);
    while (!process->atEnd()) {
        debug("backend") << process->readLine().trimmed().constData();
    }
}

void DBusBackendWrapperPrivate::slotProcessError(QProcess::ProcessError error)
{
    Q_Q(DBusBackendWrapper);
    debug("backend") << "Child process send the error" << error;
    debug("backend") << process->errorString();
    QString lastError = QString("Child process send the error :\"%1\"").arg(process->errorString());
    q->setLastError(lastError);
    q->setStatus(DBusBackendWrapper::Invalid);
}

void DBusBackendWrapperPrivate::slotFinished(int code)
{
    Q_Q(DBusBackendWrapper);
    debug("backend") << "Finished with code" << code;
    debug("backend") << "Unregister DBus object" << dbusObjectPath.toLocal8Bit().constData();
    QDBusConnection::sessionBus().unregisterObject(dbusObjectPath, QDBusConnection::UnregisterTree);
    q->setStatus(AbstractBackendWrapper::Stopped);
}

////// End of private class //////

DBusBackendWrapper::DBusBackendWrapper(const QString &identifier, const QString &executable,
                                       const QMap<QString, QString> &arguments, QObject *parent):
    AbstractBackendWrapper(*(new DBusBackendWrapperPrivate(this)), parent)
{
    Q_D(DBusBackendWrapper);
    registerDBusTypes();

    d->identifier = identifier;
    d->executable = executable;
    d->arguments = arguments;

    d->process = new QProcess(this);
    connect(d->process, &QProcess::readyReadStandardOutput,
            d, &DBusBackendWrapperPrivate::slotReadStandardOutput);
    connect(d->process, &QProcess::readyReadStandardError,
            d, &DBusBackendWrapperPrivate::slotReadStandardError);
    connect(d->process, SKSIGNAL(QProcess, error, QProcess::ProcessError),
            d, &DBusBackendWrapperPrivate::slotProcessError);
    connect(d->process, SKSIGNAL(QProcess, finished, int),
            d, &DBusBackendWrapperPrivate::slotFinished);
}

DBusBackendWrapper::~DBusBackendWrapper()
{
    kill();
}

void DBusBackendWrapper::launch()
{
    Q_D(DBusBackendWrapper);

    if (identifier().isEmpty()) {
        setLastError("No identifier was set");
        setStatus(Invalid);
        return;
    }

    QByteArray object = QCryptographicHash::hash(identifier().toLocal8Bit(),
                                                 QCryptographicHash::Md5);
    QString dbusIdentifier = QString::fromLocal8Bit(object.toHex());

    // Register to DBus
    d->dbusObjectPath = DBUS_BACKEND_PATH_PREFIX;
    d->dbusObjectPath.append(dbusIdentifier);

    new Pt2Adaptor(this);
    if (!QDBusConnection::sessionBus().registerObject(d->dbusObjectPath, this)) {
        setLastError(QString("Failed to register object on path %1").arg(d->dbusObjectPath));
        setStatus(Invalid);
        return;
    }

    // Launch the backend
    setStatus(Launching);
    d->process->setWorkingDirectory(APPLICATION_FOLDER);
    QString trueExecutable = executable();
    trueExecutable.replace("$PROVIDER", QString(PROVIDER_PATH) + " --plugin ");
    trueExecutable.append(QString(" --identifier %1 ").arg(dbusIdentifier));

    debug("dbus-backend-wrapper") << "starting" << trueExecutable;

    d->process->start(trueExecutable);
}

void DBusBackendWrapper::stop()
{
    Q_D(DBusBackendWrapper);
    if (d->process->state() == QProcess::NotRunning) {
        return;
    }

    debug("dbus-backend-wrapper") << "Stop backend for"
                                  << d->dbusObjectPath.toLocal8Bit().constData()
                                  << "with pid" << d->process->pid();

    setStatus(Stopping);
    d->process->terminate();
}

void DBusBackendWrapper::waitForStopped()
{
    Q_D(DBusBackendWrapper);
    d->process->waitForFinished(5000);
    return;
}

void DBusBackendWrapper::kill()
{
    Q_D(DBusBackendWrapper);
    if (d->process->state() == QProcess::NotRunning) {
        return;
    }

    debug("dbus-backend-wrapper") << "Kill backend for"
                                  << d->dbusObjectPath.toLocal8Bit().constData()
                                  << "with pid" << d->process->pid();
    d->process->kill();
    d->process->waitForFinished(-1);
    setStatus(Stopped);
}

void DBusBackendWrapper::registerBackend(const QStringList &capabilities, const QString &copyright)
{
    Q_D(DBusBackendWrapper);
    debug("dbus-backend-wrapper") << "Begin registration of backend for"
                                  << d->dbusObjectPath.toLocal8Bit().constData();

    if (status() == Stopping || status() == Stopped) {
        warning("dbus-backend-wrapper") << "Backend for"
                                        << d->dbusObjectPath.toLocal8Bit().constData()
                                        << "is registering while not yet launched";
        kill();
        setLastError("Backend is registering while not yet launched");
        setStatus(Invalid);
        return;
    }

    if (status() == Launched) {
        warning("dbus-backend-wrapper") << "Backend for"
                                        << d->dbusObjectPath.toLocal8Bit().constData()
                                        << "is registering twice";
        kill();
        setLastError("Backend is registering twice");
        setStatus(Invalid);
        return;
    }

    if (d->status == Invalid) {
        warning("dbus-backend-wrapper") << "Backend for"
                                        << d->dbusObjectPath.toLocal8Bit().constData()
                                        << "is registering more than twice";
        return;
    }

    debug("dbus-backend-wrapper") << "End registration of backend for"
                                  << d->dbusObjectPath.toLocal8Bit().constData();
    debug("dbus-backend-wrapper") << "Capabilities of backend retrieved for"
                                  << d->dbusObjectPath.toLocal8Bit().constData();
    debug("dbus-backend-wrapper") << "List of capabilities:";
    debug("dbus-backend-wrapper") << capabilities;
    debug("dbus-backend-wrapper") << "Copyright of backend retrieved for"
                                  << d->dbusObjectPath.toLocal8Bit().constData();

    setBackendProperties(capabilities, copyright);
    setStatus(Launched);
}
QString DBusBackendWrapper::requestRealTimeSuggestedStations(const QString &partialStation){
    QString request = createRequest(RealTime_SuggestStationFromStringType);
    emit realTimeSuggestedStationsRequested(request, partialStation);
    return request;
}
QString DBusBackendWrapper::requestRealTimeRidesFromStation(const PT2::Station &station){
    QString request = createRequest(RealTime_RidesFromStationType);
    emit realTimeRidesFromStationRequested(request, station);
    return request;
}
QString DBusBackendWrapper::requestRealTimeSuggestedLines(const QString &partialLine){
    QString request = createRequest(RealTime_SuggestLineFromStringType);
    emit realTimeSuggestedLinesRequested(request, partialLine);
    return request;
}

}

#include "dbusbackendwrapper.moc"
