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

#ifndef PT2_DBUSHELPER_H
#define PT2_DBUSHELPER_H

/**
 * @file dbushelper.h
 * @short Definition of helper functions used by DBus interface
 */

class QDBusArgument;

namespace PT2
{

class TransportationObject;
class Company;
class Line;
class Ride;
class Station;

/**
 * @brief Marshall a transportation object to a DBus argument
 *
 * This method is used to marshall a transportation object
 * to a DBus argument, making it transportable through DBus.
 *
 * A transportation object is marshalled as (a{sv}sa{sv}), where
 * the first map contains the disambiguation parametrs, the string
 * contains the name, and the second variant map contains the
 * properties.
 *
 * @param[out] argument DBus argument.
 * @param[in] transportationObject transportation object.
 * @return DBus argument containing the transportation object.
 */
QDBusArgument & transportationObjectToDBus(QDBusArgument &argument,
                                           const TransportationObject &transportationObject);
/**
 * @brief Demarshall a transportation object from a DBus argument
 *
 * This method is used to demarshall a transportation object
 * from a DBus argument.
 *
 * A transportation object is demarshalled from (a{sv}sa{sv}), where
 * the first map contains the disambiguation parametrs, the string
 * contains the name, and the second variant map contains the
 * properties.
 *
 * @param[in] argument DBus argument.
 * @param[out] transportationObject transportation object.
 * @return DBus argument without the transportation object.
 */
const QDBusArgument & transportationObjectFromDBus(const QDBusArgument &argument,
                                                   TransportationObject &transportationObject);

/**
 * @brief Marshall a company
 * @param[out] argument DBus argument.
 * @param[in] company company.
 * @return DBus argument containing the company.
 */
QDBusArgument & operator<<(QDBusArgument &argument, const Company &company);
/**
 * @brief Demarshall a company
 * @param[in] argument DBus argument.
 * @param[out] company company.
 * @return DBus argument without the company.
 */
const QDBusArgument & operator>>(const QDBusArgument &argument, Company &company);
/**
 * @brief Marshall a line
 * @param[out] argument DBus argument.
 * @param[in] line line.
 * @return DBus argument containing the line.
 */
QDBusArgument & operator<<(QDBusArgument &argument, const Line &line);
/**
 * @brief Demarshall a line
 * @param[in] argument DBus argument.
 * @param[out] line line.
 * @return DBus argument without the line.
 */
const QDBusArgument & operator>>(const QDBusArgument &argument, Line &line);
/**
 * @brief Marshall a ride
 * @param[out] argument DBus argument.
 * @param[in] ride ride.
 * @return DBus argument containing the ride.
 */
QDBusArgument & operator<<(QDBusArgument &argument, const Ride &ride);
/**
 * @brief Demarshall a ride
 * @param[in] argument DBus argument.
 * @param[out] ride ride.
 * @return DBus argument without the ride.
 */
const QDBusArgument & operator>>(const QDBusArgument &argument, Ride &ride);
/**
 * @brief Marshall a station
 * @param[out] argument DBus argument.
 * @param[in] station station.
 * @return DBus argument containing the station.
 */
QDBusArgument & operator<<(QDBusArgument &argument, const Station &station);
/**
 * @brief Demarshall a station
 * @param[in] argument DBus argument.
 * @param[out] station station.
 * @return DBus argument without the station.
 */
const QDBusArgument & operator>>(const QDBusArgument &argument, Station &station);

/**
 * @brief Register DBus types
 *
 * This method is used to register complex types to the
 * DBus type system. This method should be called before
 * making any DBus call involving these complex types.
 */
void registerDBusTypes();

}

#endif // PT2_DBUSHELPER_H
