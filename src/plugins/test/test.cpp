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
 * @internal
 * @file test.cpp
 * @short Implementation of PT2::Provider::Test
 */

#include "test.h"

#include "debug.h"
#include "errorid.h"
#include "capabilitiesconstants.h"
#include "base/station.h"

namespace PT2
{

namespace Provider
{

Test::Test(QObject *parent) :
    ProviderPluginObject(parent)
{
}

QStringList Test::capabilities() const
{
    QStringList capabilities;
    capabilities.append(CAPABILITY_REAL_TIME_SUGGEST_STATION_FROM_STRING);
    return capabilities;
}

QString Test::copyright() const
{
    return "Test, no copyright.";
}

void Test::retrieveRealTimeSuggestedStations(const QString &request, const QString &partialStation)
{
    QList<Station> stations;
    Station station1 = Station("org.SfietKonstantin.test.station1", QVariantMap(), "Test1",
                               QVariantMap());
    stations.append(station1);

    if (partialStation == "test") {
        Station station2 = Station("org.SfietKonstantin.test.station2", QVariantMap(), "Test2",
                                   QVariantMap());
        stations.append(station2);
    }

    emit realTimeSuggestedStationsRetrieved(request, stations);
}

}

}
