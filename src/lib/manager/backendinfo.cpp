/****************************************************************************************
 * Copyright (C) 2012 Lucien XU <sfietkonstantin@free.fr>                               *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 3 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

/**
 * @file backendinfo.cpp
 * @short Implementation of PT2::BackendInfo
 */

#include "backendinfo.h"
#include <MDesktopEntry>
#include <QtCore/QSharedData>

namespace PT2
{

/**
 * @internal
 * @brief DESKTOP_FILE_GROUP
 *
 * Used in PT2::BackendInfo.
 */
static const char *DESKTOP_FILE_GROUP = "Desktop Entry";
/**
 * @internal
 * @brief DESKTOP_FILE_TYPE_VALUE
 *
 * Used in PublicTransportation::BackendInfo.
 */
static const char *DESKTOP_FILE_TYPE_VALUE = "Service";
/**
 * @internal
 * @brief DESKTOP_FILE_BACKENDINFO_ID
 *
 * Used in PublicTransportation::BackendInfo.
 */
static const char *DESKTOP_FILE_BACKENDINFO_ID = "X-PublicTransportation-BackendInfo-Id";
/**
 * @internal
 * @brief DESKTOP_FILE_BACKENDINFO_COUNTRY
 *
 * Used in PublicTransportation::BackendInfo.
 */
static const char *DESKTOP_FILE_BACKENDINFO_COUNTRY = "X-PT2-BackendInfo-Country";
/**
 * @internal
 * @brief DESKTOP_FILE_BACKENDINFO_CITIES
 *
 * Used in PublicTransportation::BackendInfo.
 */
static const char *DESKTOP_FILE_BACKENDINFO_CITIES = "X-PublicTransportation-BackendInfo-Cities";
/**
 * @internal
 * @brief DESKTOP_FILE_BACKENDINFO_AUTHOR
 *
 * Used in PublicTransportation::BackendInfo.
 */
static const char *DESKTOP_FILE_BACKENDINFO_AUTHOR = "X-PublicTransportation-BackendInfo-Author";
/**
 * @internal
 * @brief DESKTOP_FILE_BACKENDINFO_EMAIL
 *
 * Used in PublicTransportation::BackendInfo.
 */
static const char *DESKTOP_FILE_BACKENDINFO_EMAIL = "X-PublicTransportation-BackendInfo-Email";
/**
 * @internal
 * @brief DESKTOP_FILE_BACKENDINFO_WEBSITE
 *
 * Used in PublicTransportation::BackendInfo.
 */
static const char *DESKTOP_FILE_BACKENDINFO_WEBSITE = "X-PublicTransportation-BackendInfo-Website";
/**
 * @internal
 * @brief DESKTOP_FILE_BACKENDINFO_VERSION
 *
 * Used in PublicTransportation::BackendInfo.
 */
static const char *DESKTOP_FILE_BACKENDINFO_VERSION = "X-PublicTransportation-BackendInfo-Version";

BackendInfoPrivate::BackendInfoPrivate()
    : QSharedData()
{
}

BackendInfoPrivate::BackendInfoPrivate(const BackendInfoPrivate &other)
    : QSharedData(other), icon(other.icon), name(other.name)
    , description(other.description), executable(other.executable)
    , identifier(other.identifier), author(other.author), email(other.email)
    , website(other.website), version(other.version), country(other.country)
    , cities(other.cities)
{
}

BackendInfoPrivate::~BackendInfoPrivate()
{
}

////// End of private class //////

BackendInfo::BackendInfo():
    d(new BackendInfoPrivate)
{
}

BackendInfo::BackendInfo(const BackendInfo &other)
    : d(other.d)
{
}

BackendInfo::BackendInfo(const QString &file):
    d(new BackendInfoPrivate)
{
    // Parse desktop file
    MDesktopEntry parser (file);

    // Check if it is valid
    if (parser.type() != DESKTOP_FILE_TYPE_VALUE) {
        return;
    }

    if (parser.exec().isEmpty()) {
        return;
    }

    d->icon = parser.icon();
    d->name = parser.name();
    d->description = parser.comment();
    d->country = parser.value(DESKTOP_FILE_GROUP, DESKTOP_FILE_BACKENDINFO_COUNTRY);
    QString cities = parser.value(DESKTOP_FILE_GROUP, DESKTOP_FILE_BACKENDINFO_CITIES);
    d->cities = cities.split(",");
    d->executable = parser.exec();
    d->identifier = parser.value(DESKTOP_FILE_GROUP, DESKTOP_FILE_BACKENDINFO_ID);
    d->author = parser.value(DESKTOP_FILE_GROUP, DESKTOP_FILE_BACKENDINFO_AUTHOR);
    d->email = parser.value(DESKTOP_FILE_GROUP, DESKTOP_FILE_BACKENDINFO_EMAIL);
    d->website = parser.value(DESKTOP_FILE_GROUP, DESKTOP_FILE_BACKENDINFO_WEBSITE);
    d->version = parser.value(DESKTOP_FILE_GROUP, DESKTOP_FILE_BACKENDINFO_VERSION);
}

BackendInfo::~BackendInfo()
{
}

bool BackendInfo::isValid() const
{
    return !d->executable.isEmpty() && !d->identifier.isEmpty();
}

QString BackendInfo::icon() const
{
    return d->icon;
}

QString BackendInfo::name() const
{
    return d->name;
}

QString BackendInfo::description() const
{
    return d->description;
}

QString BackendInfo::executable() const
{
    return d->executable;
}

QString BackendInfo::backendIdentifier() const
{
    return d->identifier;
}

QString BackendInfo::backendCountry() const
{
    return d->country;
}

QStringList BackendInfo::backendCities() const
{
    return d->cities;
}

QString BackendInfo::backendAuthor() const
{
    return d->author;
}

QString BackendInfo::backendEmail() const
{
    return d->email;
}

QString BackendInfo::backendWebsite() const
{
    return d->website;
}

QString BackendInfo::backendVersion() const
{
    return d->version;
}

}
