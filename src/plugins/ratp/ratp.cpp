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
 * @file ratp.cpp
 * @short Implementation of PT2::Provider::Ratp
 */

#include "ratp.h"

#include "debug.h"
#include "errorid.h"
#include "capabilitiesconstants.h"
#include "base/line.h"
#include "base/ride.h"
#include "base/station.h"
#include "base/companynodedata.h"
#include "base/ridenodedata.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

namespace PT2
{

namespace Provider
{

static const char *IDENTIFIER_TEMPLATE = "org.SfietKonstantin.pt2.ratp/%1";
static const char *DB_IDENTIFIER_KEY = "db_identifier";
static const char *RATP_IDENTIFIER_KEY = "ratp_identifier";

static inline QString unaccent(const QString &string)
{
    QString normalized = string.normalized(QString::NormalizationForm_D);
    QString out;
    for (int i = 0; i < normalized.count(); i++) {
        // strip diacritic marks
        if (normalized.at(i).category() != QChar::Mark_NonSpacing
            && normalized.at(i).category() != QChar::Mark_SpacingCombining) {
            out.append(normalized.at(i));
        }
    }
    return out;
}

Ratp::Ratp(QObject *parent) :
    ProviderPluginObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(QString("%1/ratp/ratp.db").arg(PLUGIN_FOLDER));
    if (!m_db.open()) {
        return;
    }
}

Ratp::~Ratp()
{
    m_db.close();
}

QStringList Ratp::capabilities() const
{
    QStringList capabilities;
    capabilities.append(CAPABILITY_REAL_TIME_SUGGEST_STATION_FROM_STRING);
    capabilities.append(CAPABILITY_REAL_TIME_SUGGEST_LINE_FROM_STRING);
    capabilities.append(CAPABILITY_REAL_TIME_RIDES_FROM_STATION);
    return capabilities;
}

QString Ratp::copyright() const
{
    return tr("This provider uses wap.ratp.fr.") + "\n"
            + QString::fromUtf8("RATP - Tous droits réservés");
}

void Ratp::retrieveRealTimeSuggestedStations(const QString &request, const QString &partialStation)
{
    QList<Station> stations;
    if (partialStation.count() < 3) {
        emit realTimeSuggestedStationsRetrieved(request, stations);
        return;
    }

    QString stationQuery = unaccent(partialStation);
    stationQuery.append("%");
    QSqlQuery query (m_db);
    query.prepare("SELECT id, name FROM stations WHERE nameUnaccented LIKE :station "\
                  "ORDER BY nameUnaccented");
    query.bindValue(":station", stationQuery);
    if (!query.exec()) {
        warning("ratp") << "Error:" << query.lastError().text() << query.lastQuery();
        emit errorRetrieved(request, ERROR_BACKEND_WARNING, "Failed to query stations from DB.");
        return;
    }

    while (query.next()) {
        QString identifier = QString(IDENTIFIER_TEMPLATE).arg(query.value(0).toInt());
        QVariantMap internal;
        internal.insert(DB_IDENTIFIER_KEY, query.value(0).toInt());
        Station station (identifier, internal, query.value(1).toString(), QVariantMap());
        stations.append(station);
    }

    query.prepare("SELECT id, name FROM stations WHERE nameUnaccented LIKE :station1 "\
                  "AND nameUnaccented NOT LIKE :station2 ORDER BY nameUnaccented");
    QString otherStationQuery = unaccent(partialStation);
    otherStationQuery.prepend("%");
    otherStationQuery.append("%");

    query.bindValue(":station1", otherStationQuery);
    query.bindValue(":station2", stationQuery);
    if (!query.exec()) {
        warning("ratp") << "Error:" << query.lastError().text() << query.lastQuery();
        emit errorRetrieved(request, ERROR_BACKEND_WARNING, "Failed to query stations from DB.");
        return;
    }

    while (query.next()) {
        QString identifier = QString(IDENTIFIER_TEMPLATE).arg(query.value(0).toInt());
        QVariantMap internal;
        internal.insert(DB_IDENTIFIER_KEY, query.value(0).toInt());
        Station station (identifier, internal, query.value(1).toString(), QVariantMap());
        stations.append(station);
    }

    emit realTimeSuggestedStationsRetrieved(request, stations);
}

void Ratp::retrieveRealTimeRidesFromStation(const QString &request, const Station &station)
{
    QList<CompanyNodeData> rides;
    int stationDbId = station.internal().value(DB_IDENTIFIER_KEY).toInt();
    debug("ratp") << "Using station id" << stationDbId;

    Company company (QString(IDENTIFIER_TEMPLATE).arg(0), QVariantMap(), tr("RATP"), QVariantMap());

    QList<LineNodeData> lines;
    QMap<QString, LineNodeData> lineMap;


    QSqlQuery query (m_db);
    query.prepare("SELECT DISTINCT id, lineCode, lineName, directionCode, directionName, "\
                  "link_station_ride.stationCode FROM rides "\
                  "INNER JOIN link_station_ride ON rides.id = link_station_ride.rideId "\
                  "WHERE link_station_ride.stationId = :stationId");
    query.bindValue(":stationId", stationDbId);

    if (!query.exec()) {
        warning("ratp") << "Error:" << query.lastError().text() << query.lastQuery();
        emit errorRetrieved(request, ERROR_BACKEND_WARNING, "Failed to query stations from DB.");
        return;
    }

    // Create lines and populate them with the ride
    while (query.next()) {
        QString lineCode = query.value(1).toString();
        if (!lineMap.contains(lineCode)) {
            QString lineName = query.value(2).toString();
            debug("ratp") << "Found line" << lineName;
            QString lineIdentifier = QString(IDENTIFIER_TEMPLATE).arg(lineCode);
            QVariantMap lineInternal;
            lineInternal.insert(RATP_IDENTIFIER_KEY, lineCode);
            Line line (lineIdentifier, lineInternal, lineName, QVariantMap());
            lineMap.insert(lineCode, LineNodeData (line));
        }

        // Ride
        QString rideName = query.value(4).toString();
        QString rideIdentifier = QString(IDENTIFIER_TEMPLATE).arg(query.value(0).toInt());
        QVariantMap rideInternal;
        rideInternal.insert(RATP_IDENTIFIER_KEY, query.value(3).toString());
        Ride ride (rideIdentifier, rideInternal, rideName, QVariantMap());

        // Station
        QVariantMap stationInternal = station.internal();
        stationInternal.insert(RATP_IDENTIFIER_KEY, query.value(5).toString());
        Station newStation (station.identifier(), stationInternal, station.name(),
                            station.properties());
        QList<Station> stationList;
        stationList.append(newStation);

        // Just in case
        if (lineMap.contains(lineCode)) {
            QList<RideNodeData> rideNodeDataList = lineMap[lineCode].rideNodeDataList();
            rideNodeDataList.append(RideNodeData(ride, stationList));
            lineMap[lineCode].setRideNodeDataList(rideNodeDataList);
        }
    }

    foreach (const LineNodeData &line, lineMap) {
        lines.append(line);
    }

    rides.append(CompanyNodeData(company, lines));

    emit realTimeRidesFromStationRetrieved(request, rides);
}

void Ratp::retrieveRealTimeSuggestedLines(const QString &request, const QString &partialLine)
{
    QList<Line> lines;
    emit realTimeSuggestedLinesRetrieved(request, lines);
}

}

}
