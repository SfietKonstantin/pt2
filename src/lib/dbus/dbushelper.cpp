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
 * @file dbushelper.cpp
 * @short Implementation of helper functions used by DBus interface
 */

#include "dbushelper.h"
#include "base/company.h"
#include "base/line.h"
#include "base/ride.h"
#include "base/station.h"
#include "base/companynodedata.h"
#include "base/linenodedata.h"
#include "base/ridenodedata.h"

#include <QtDBus/QtDBus>
#include <QtDBus/QDBusArgument>

namespace PT2
{

QDBusArgument & transportationObjectToDBus(QDBusArgument &argument,
                                           const TransportationObject &transportationObject)
{
    argument.beginStructure();

    // Identifier
    argument << transportationObject.identifier();

    // Internal map
    argument.beginMap(qMetaTypeId<QString>(), qMetaTypeId<QDBusVariant>());
    QVariantMap map = transportationObject.internal();
    foreach (QString key, map.keys()) {
        argument.beginMapEntry();
        argument << key << QDBusVariant(map.value(key));
        argument.endMapEntry();
    }
    argument.endMap();

    // Name
    argument << transportationObject.name();

    // Properties map
    argument.beginMap(qMetaTypeId<QString>(), qMetaTypeId<QDBusVariant>());
    map = transportationObject.properties();
    foreach (QString key, map.keys()) {
        argument.beginMapEntry();
        argument << key << QDBusVariant(map.value(key));
        argument.endMapEntry();
    }
    argument.endMap();
    argument.endStructure();

    return argument;
}

const QDBusArgument & transportationObjectFromDBus(const QDBusArgument &argument,
                                                   TransportationObject &transportationObject)
{
    argument.beginStructure();

    // Identifier
    QString identifier;
    argument >> identifier;

    transportationObject.setIdentifier(identifier);

    // Internal map
    QVariantMap map;
    argument.beginMap();
    while (!argument.atEnd()) {
        argument.beginMapEntry();
        QString key;
        QDBusVariant value;
        argument >> key;
        argument >> value;
        map.insert(key, value.variant());
        argument.endMapEntry();
    }
    argument.endMap();
    transportationObject.setInternal(map);

    // Name
    QString name;
    argument >> name;
    transportationObject.setName(name);

    // Properties map
    argument.beginMap();
    while (!argument.atEnd()) {
        argument.beginMapEntry();
        QString key;
        QDBusVariant value;
        argument >> key;
        argument >> value;
        map.insert(key, value.variant());
        argument.endMapEntry();
    }
    argument.endMap();
    transportationObject.setProperties(map);
    argument.endStructure();

    return argument;
}

QDBusArgument & operator<<(QDBusArgument &argument, const Company &company)
{
    return transportationObjectToDBus(argument, company);
}

const QDBusArgument & operator>>(const QDBusArgument &argument, Company &company)
{
    return transportationObjectFromDBus(argument, company);
}

QDBusArgument & operator<<(QDBusArgument &argument, const Line &line)
{
    return transportationObjectToDBus(argument, line);
}

const QDBusArgument & operator>>(const QDBusArgument &argument, Line &line)
{
    return transportationObjectFromDBus(argument, line);
}

QDBusArgument & operator<<(QDBusArgument &argument, const Ride &ride)
{
    return transportationObjectToDBus(argument, ride);
}

const QDBusArgument & operator>>(const QDBusArgument &argument, Ride &ride)
{
    return transportationObjectFromDBus(argument, ride);
}

QDBusArgument & operator<<(QDBusArgument &argument, const Station &station)
{
    return transportationObjectToDBus(argument, station);
}

const QDBusArgument & operator>>(const QDBusArgument &argument, Station &station)
{
    return transportationObjectFromDBus(argument, station);
}

QDBusArgument & operator<<(QDBusArgument &argument, const CompanyNodeData &companyNodeData)
{
    argument.beginStructure();
    argument << companyNodeData.company();
    argument.beginArray();
    foreach (const LineNodeData &lineNodeData, companyNodeData.lineNodeDataList()) {
        argument << lineNodeData;
    }
    argument.endArray();
    argument.endStructure();

    return argument;
}

const QDBusArgument & operator>>(const QDBusArgument &argument, CompanyNodeData &companyNodeData)
{
    argument.beginStructure();
    Company company;
    argument >> company;
    companyNodeData.setCompany(company);

    argument.beginArray();
    QList<LineNodeData> lineNodeDataList;
    while (!argument.atEnd()) {
        LineNodeData lineNodeData;
        argument >> lineNodeData;
        lineNodeDataList.append(lineNodeData);
    }
    argument.endArray();
    companyNodeData.setLineNodeDataList(lineNodeDataList);
    argument.endStructure();

    return argument;
}

QDBusArgument & operator<<(QDBusArgument &argument, const LineNodeData &lineNodeData)
{
    argument.beginStructure();
    argument << lineNodeData.line();
    argument.beginArray();
    foreach (const RideNodeData &rideNodeData, lineNodeData.rideNodeDataList()) {
        argument << rideNodeData;
    }
    argument.endArray();
    argument.endStructure();

    return argument;
}

const QDBusArgument & operator>>(const QDBusArgument &argument, LineNodeData &lineNodeData)
{
    argument.beginStructure();
    Line line;
    argument >> line;
    lineNodeData.setLine(line);

    argument.beginArray();
    QList<RideNodeData> rideNodeDataList;
    while (!argument.atEnd()) {
        RideNodeData rideNodeData;
        argument >> rideNodeData;
        rideNodeDataList.append(rideNodeData);
    }
    argument.endArray();
    lineNodeData.setRideNodeDataList(rideNodeDataList);
    argument.endStructure();

    return argument;
}

QDBusArgument & operator<<(QDBusArgument &argument, const RideNodeData &rideNodeData)
{
    argument.beginStructure();
    argument << rideNodeData.ride();
    argument.beginArray();
    foreach (const Station &station, rideNodeData.stationList()) {
        argument << station;
    }
    argument.endArray();
    argument.endStructure();

    return argument;
}

const QDBusArgument & operator>>(const QDBusArgument &argument, RideNodeData &rideNodeData)
{
    argument.beginStructure();
    Ride ride;
    argument >> ride;
    rideNodeData.setRide(ride);

    argument.beginArray();
    QList<Station> stationList;
    while (!argument.atEnd()) {
        Station station;
        argument >> station;
        stationList.append(station);
    }
    argument.endArray();
    rideNodeData.setStationList(stationList);
    argument.endStructure();

    return argument;
}

void registerDBusTypes()
{
    qDBusRegisterMetaType<Company>();
    qDBusRegisterMetaType<QList<Company> >();
    qDBusRegisterMetaType<Line>();
    qDBusRegisterMetaType<QList<Line> >();
    qDBusRegisterMetaType<Ride>();
    qDBusRegisterMetaType<QList<Ride> >();
    qDBusRegisterMetaType<Station>();
    qDBusRegisterMetaType<QList<Station> >();
    qDBusRegisterMetaType<CompanyNodeData>();
    qDBusRegisterMetaType<QList<CompanyNodeData> >();
    qDBusRegisterMetaType<LineNodeData>();
    qDBusRegisterMetaType<QList<LineNodeData> >();
    qDBusRegisterMetaType<RideNodeData>();
    qDBusRegisterMetaType<QList<RideNodeData> >();

}

}
