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

#ifndef PT2_ABSTRACTBACKENDWRAPPER_H
#define PT2_ABSTRACTBACKENDWRAPPER_H

/**
 * @file abstractbackendwrapper.h
 * @short Definition of PT2::AbstractBackendWrapper
 */


#include "pt2_global.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace PT2
{

class Company;
class Line;
class Ride;
class Station;
class AbstractBackendWrapperPrivate;

/**
 * @brief Base class for a backend wrapper
 *
 * This class provides a base for any backend wrapper.
 * A backend wrapper is an interface between a backend
 * that is usually a process that is spawned, and that is
 * used to perform specific tasks in an asynchronous ways.
 *
 * This class is used to perform the interface between the
 * spawned backend and the applications that is used to
 * display informations.
 *
 * In order to do that, the backend wrapper provides methods
 * like launch(), stop() and kill() that should be implemented,
 * as well as a set of properties, like status(), that is used
 * to get the current backend status.
 *
 * In order to be managed more easily, all backend wrappers have
 * an identifier, that is passed in the constructor, and that
 * should be unique for each backend. This identifier can also
 * be passed to the backend if needed.
 *
 * @section capabilities Capabilities
 *
 * A backend have capabilities that might vary from backend to
 * backend. Those capabilities provides what the backend can
 * deliver. A sane application should never call methods that
 * are related to a capability that is not provided by the backend,
 * as the behaviour might be unknown.
 *
 * @section managingBackend Managing backend
 *
 * The backend wrapper is used to manage backend and provide
 * a neat interface of the backend to the application. When
 * a backend is launched, the status should be set to
 * AbstractBackendWrapper::Launching. When it is being stopped,
 * it should be set to AbstractBackendWrapper::Stopping. While the
 * backend ping the wrapper back, it should also register its
 * capabilities. While an error occur in the backend (like failing
 * to register a DBus service, for example), the
 * AbstractBackendWrapper::Invalid status should be set.
 *
 * Three helper methods are used to help implementing backend wrapper:
 * - setStatus()
 * - setLastError()
 * - setBackendProperties()
 *
 * @section relayingReplies Relaying replies
 *
 * This class is also used to relay replies from the backend,
 * using specific combinaison of signals and slots. When the
 * backend replied, a slot like \b registerAbc should be called.
 * This slot will emit \b abcRegistered. Here is the list of
 * signals / slots used for the relay
 * - registerError()
 * - registerRealTimeSuggestedStations()
 * and
 * - errorRegistered()
 * - realTimeSuggestedStationsRegistered()
 *
 * This class also provides interfaces for implementing some capabilities
 * of the providers, that should be implemented in subclasses. They are all
 * of the form \b requestAbc.
 * - requestRealTimeSuggestedStations()
 *
 * All these requests returns a request identifier, and all responses will
 * provide the same identifier, in order to identify the request more easily.
 *
 * Implementing requests can be done by calling createRequest(). This method
 * provides a request identifier, and register the request as pending. When
 * requests are answered, they are removed. The abstract backend wrapper can
 * then takes care of request tracking.
 *
 * Remark that there is no request for capabilities or copyright. It is
 * because registering capabilities and copyright is something that backends
 * should do automatically, in order to be validated. Subclasses should implement
 * a method to perform this registration (that should use setBackendProperties())
 * and that should set the status to AbstractBackendWrapper::Launched.
 *
 */
class PT2_EXPORT AbstractBackendWrapper: public QObject
{
    Q_OBJECT
    /**
     * @short Status
     */
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    /**
     * @brief Enumeration describing backend status
     */
    enum Status {
        /**
         * @short The backend is stopped
         */
        Stopped,
        /**
         * @short The backend is launching
         */
        Launching,
        /**
         * @short The backend is launched
         */
        Launched,
        /**
         * @short The backend is stopping
         */
        Stopping,
        /**
         * @short The backend is in an invalid state
         */
        Invalid
    };

    /**
     * @brief Enumeration describing request types
     */
    enum RequestType {
        /**
         * @short Request suggested stations for real time information
         */
        RealTime_SuggestStationFromStringType
    };

    /**
     * @brief Default constructor
     *
     * A set of properties can be passed to the constructor.
     * Those arguments can be used in the implementation to
     * provide, for example, additional command line arguments,
     * to the backend that controlled by this wrapper.
     *
     * @param identifier identifier for this backend wrapper.
     * @param executable command line that launch the backend.
     * @param arguments list of arguments.
     * @param parent parent object.
     */
    explicit AbstractBackendWrapper(const QString &identifier, const QString &executable,
                                    const QMap<QString, QString> &arguments, QObject *parent = 0);
    /**
     * @brief Destructor
     */
    virtual ~AbstractBackendWrapper();
    /**
     * @brief Identifier
     * @return identifier.
     */
    QString identifier() const;
    /**
     * @brief Status
     * @return status.
     */
    Status status() const;
    /**
     * @brief Last error
     * @return last error.
     */
    QString lastError() const;
    /**
     * @brief Capabilities
     * @return capabilities.
     */
    QStringList capabilities() const;
    /**
     * @brief Copyright
     * @return copyright.
     */
    QString copyright() const;
    /**
     * @brief Request suggested stations for real time information
     * @param partialStation partial station name.
     * @return request identifier.
     */
    virtual QString requestRealTimeSuggestedStations(const QString &partialStation) = 0;
public Q_SLOTS:
    /**
     * @brief Launch the backend
     *
     * Launch the backend. Implementation can use the
     * executable and the arguments that are passed
     * while an instance of the backend wrapper is created
     * to perform custom launches.
     */
    virtual void launch() = 0;
    /**
     * @brief Stop the backend
     *
     * Stops the backend. The stop is asynchronous, and can
     * occur a bit after.
     */
    virtual void stop() = 0;
    /**
     * @brief Wait for stopped
     *
     * This method can be reimplemented to wait for the backend
     * to finish cleanly.
     */
    virtual void waitForStopped();
    /**
     * @brief Kill the backend
     *
     * Kills the backend. The backend should be immediately
     * stopped, as this method is called either in emergency
     * situations.
     */
    virtual void kill() = 0;
    /**
     * @brief Register error
     *
     * This method is used to reply to any request and to note that there were an
     * error during the request. Error categories can be found in file @ref errorid.h
     *
     * @param request request identifier.
     * @param errorId a predefined string that provides the error category.
     * @param error a human-readable string describing the error.
     */
    void registerError(const QString &request, const QString &errorId, const QString &error);
    /**
     * @brief Register suggested stations for real time information
     *
     * This method is used to register a list of suggested stations. Returned stations are used in
     * other signals, so these stations can store additional properties. An interesting property
     * to also set is "backendName", that provides to the GUI an information about the backend
     * used for getting this station. It can be used by the user to distinguish between two
     * stations that have the same name, but are provided by different backends.
     *
     * @param request request identifier.
     * @param suggestedStationList suggested station list.
     */
    void registerRealTimeSuggestedStations(const QString &request, const QList<PT2::Station> &suggestedStationList);
Q_SIGNALS:
    /**
     * @brief Status changed
     */
    void statusChanged();
    /**
     * @brief Capabilities changed
     */
    void capabilitiesChanged();
    /**
     * @brief Copyright changed
     */
    void copyrightChanged();
    /**
     * @brief Error registered
     *
     * This signal is used to relay registered errors.
     *
     * @param request request identifier.
     * @param errorId a predefined string that provides the error category.
     * @param error a human-readable string describing the error.
     */
    void errorRegistered(const QString &request, const QString &errorId, const QString &error);
    /**
     * @brief Suggested stations registered for real time information
     *
     * This signal is used to relay registered suggested stations for real time information
     *
     * @param request request identifier.
     * @param suggestedStationList suggested station list.
     */
    void realTimeSuggestedStationsRegistered(const QString &request, const QList<PT2::Station> &suggestedStationList);
protected:
    /**
     * @brief D-pointer based constructor
     * @param dd d-pointer.
     * @param parent parent object.
     */
    explicit AbstractBackendWrapper(AbstractBackendWrapperPrivate &dd, QObject *parent);
    /**
     * @brief Executable
     * @return executable.
     */
    QString executable() const;
    /**
     * @brief Arguments
     * @return arguments.
     */
    QMap<QString, QString> arguments() const;
    /**
     * @brief Set status
     * @param status status to set.
     */
    void setStatus(Status status);
    /**
     * @brief Set last error
     * @param lastError error to set.
     */
    void setLastError(const QString &lastError);
    /**
     * @brief Set backend properties
     *
     * This method is used to set the properties of a
     * backend, that are the capabilities and the
     * copyright.
     *
     * @param capabilities capabilities to set.
     * @param copyright copyright to set.
     */
    void setBackendProperties(const QStringList &capabilities, const QString &copyright);
    /**
     * @brief Create request
     *
     * This method is used to create a request, and register it in the
     * backend wrapper. This method is useful whenever a request is
     * performed, in order to get a request identifier.
     *
     * @param requestType request type.
     * @return request identifier.
     */
    QString createRequest(RequestType requestType);

    /**
     * @brief D-pointer
     */
    QScopedPointer<AbstractBackendWrapperPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(AbstractBackendWrapper)
};

}

#endif // PT2_ABSTRACTBACKENDWRAPPER_H
