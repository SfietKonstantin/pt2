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

#ifndef PT2_PROVIDER_TEST_H
#define PT2_PROVIDER_TEST_H

/**
 * @internal
 * @file ratp.h
 * @short Definition of PT2::Provider::Ratp
 */

#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include "provider/providerpluginobject.h"

namespace PT2
{

namespace Provider
{

/**
 * @internal
 * @brief RATP provider
 *
 * This provider uses wap.ratp.fr to fetch
 * information about public transportation in
 * Paris.
 */
class Ratp : public ProviderPluginObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.SfietKonstantin.pt2.Plugin.ProviderPluginInterface")
    Q_INTERFACES(PT2::ProviderPluginInterface)
public:
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit Ratp(QObject *parent = 0);
    virtual ~Ratp();
    QStringList capabilities() const;
    QString copyright() const;
public Q_SLOTS:
    void retrieveRealTimeSuggestedStations(const QString &request, const QString &partialStation);
    void retrieveRealTimeRidesFromStation(const QString &request, const Station &station);
    void retrieveRealTimeSuggestedLines(const QString &request, const QString &partialLine);
private:
    QSqlDatabase m_db;
};

}

}

#endif // PT2_PROVIDER_TEST_H
