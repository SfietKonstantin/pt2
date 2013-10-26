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
 * @file providerplugindbuswrapper.cpp
 * @short Implementation of PT2::ProviderPluginDBusWrapper
 */


#include "providerplugindbuswrapper.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QPluginLoader>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusServiceWatcher>

#include "debug.h"
#include "dbus/dbusconstants.h"
#include "dbus/dbushelper.h"
#include "dbus/generated/backenddbusproxy.h"
#include "provider/providerpluginobject.h"

namespace PT2
{

/**
 * @internal
 * @brief Private class for PT2::ProviderPluginDBusWrapper
 */
class ProviderPluginDBusWrapperPrivate: public QObject
{
    Q_OBJECT
public:
    /**
     * @internal
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit ProviderPluginDBusWrapperPrivate(QObject *parent = 0);
    /**
     * @internal
     * @brief Proxy
     */
    OrgSfietKonstantinPt2Interface *proxy;
    /**
     * @internal
     * @brief DBus object path
     */
    QString dbusObjectPath;
    /**
     * @internal
     * @brief Provider plugin
     */
    ProviderPluginObject *provider;
public Q_SLOTS:
    /**
     * @internal
     * @brief Slot for service unregistered
     * @param service service name.
     */
    void slotServiceUnregistered(const QString &service);
};

ProviderPluginDBusWrapperPrivate::ProviderPluginDBusWrapperPrivate(QObject *parent)
    : QObject(parent)
{
}

void ProviderPluginDBusWrapperPrivate::slotServiceUnregistered(const QString &service)
{
    Q_UNUSED(service)
    QCoreApplication::quit();
}

////// End of private class //////

ProviderPluginDBusWrapper::ProviderPluginDBusWrapper(const QString &identifier, QObject *parent):
    QObject(parent), d_ptr(new ProviderPluginDBusWrapperPrivate)
{
    Q_D(ProviderPluginDBusWrapper);
    registerDBusTypes();

    d->dbusObjectPath = QString(DBUS_BACKEND_PATH_PREFIX) + identifier;
    d->provider = 0;
    d->proxy = new OrgSfietKonstantinPt2Interface(DBUS_SERVICE, d->dbusObjectPath,
                                                  QDBusConnection::sessionBus(), this);
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher(DBUS_SERVICE,
                                                           QDBusConnection::sessionBus(),
                                                           QDBusServiceWatcher::WatchForUnregistration,
                                                           this);
    connect(watcher, &QDBusServiceWatcher::serviceUnregistered,
            d, &ProviderPluginDBusWrapperPrivate::slotServiceUnregistered);
}

ProviderPluginDBusWrapper::~ProviderPluginDBusWrapper()
{
}

bool ProviderPluginDBusWrapper::load(const QString &plugin)
{
    Q_D(ProviderPluginDBusWrapper);
    QDir dir (PLUGIN_FOLDER);
    if (!dir.exists(plugin)) {
        warning("provider-wrapper") << "The plugin" << plugin.toLocal8Bit().constData()
                                    << "cannot be found";
        return false;
    }

    QString pluginPath = dir.absoluteFilePath(plugin);
    QPluginLoader pluginLoader(pluginPath);
    QObject *pluginObject = pluginLoader.instance();
    if (!pluginObject) {
        warning("provider-wrapper") << "The plugin" << plugin.toLocal8Bit().constData()
                                    << "cannot be loaded";
        warning("provider-wrapper") << pluginLoader.errorString();
        return false;
    }

    d->provider = qobject_cast<ProviderPluginObject *>(pluginObject);
    if (!d->provider) {
        warning("provider-wrapper") << "The plugin" << plugin.toLocal8Bit().constData()
                                    << "is not valid";
        return false;
    }

    // Establish some connections
    connect(d->provider, &ProviderPluginObject::errorRetrieved,
            d->proxy, &OrgSfietKonstantinPt2Interface::registerError);
    connect(d->proxy, &OrgSfietKonstantinPt2Interface::realTimeSuggestedStationsRequested,
            d->provider, &ProviderPluginObject::retrieveRealTimeSuggestedStations);
    connect(d->provider, &ProviderPluginObject::realTimeSuggestedStationsRetrieved,
            d->proxy, &OrgSfietKonstantinPt2Interface::registerRealTimeSuggestedStations);
    connect(d->proxy, &OrgSfietKonstantinPt2Interface::realTimeRidesFromStationRequested,
            d->provider, &ProviderPluginObject::retrieveRealTimeRidesFromStation);
    connect(d->provider, &ProviderPluginObject::realTimeRidesFromStationRetrieved,
            d->proxy, &OrgSfietKonstantinPt2Interface::registerRealTimeRidesFromStation);
    connect(d->proxy, &OrgSfietKonstantinPt2Interface::realTimeSuggestedLinesRequested,
            d->provider, &ProviderPluginObject::retrieveRealTimeSuggestedLines);
    connect(d->provider, &ProviderPluginObject::realTimeSuggestedLinesRetrieved,
            d->proxy, &OrgSfietKonstantinPt2Interface::registerRealTimeSuggestedLines);

    // Register
    debug("provider-wrapper") << "Registration from backend with pid"
                              << QCoreApplication::applicationPid();
    d->proxy->registerBackend(d->provider->capabilities(), d->provider->copyright());

    return true;
}

}

#include "providerplugindbuswrapper.moc"
