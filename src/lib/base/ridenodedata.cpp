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

#include "ridenodedata.h"

/**
 * @file ridenodedata.cpp
 * @short Implementation of PT2::RideNodeData
 */

namespace PT2
{

RideNodeDataPrivate::RideNodeDataPrivate()
    : QSharedData()
{
}

RideNodeDataPrivate::RideNodeDataPrivate(const RideNodeDataPrivate &other)
    : QSharedData(other), ride(other.ride), stationList(other.stationList)
{
}

RideNodeDataPrivate::~RideNodeDataPrivate()
{
}

////// End of private class //////

RideNodeData::RideNodeData(const Ride &ride, const QList<Station> &stationList)
    : d(new RideNodeDataPrivate())
{
    d->ride = ride;
    d->stationList = stationList;
}

RideNodeData::RideNodeData(const RideNodeData &other)
    : d(other.d)
{
}

RideNodeData::~RideNodeData()
{
}

bool RideNodeData::operator==(const RideNodeData &other) const
{
    return (d->ride == other.ride() && d->stationList == other.stationList());
}

bool RideNodeData::operator!=(const RideNodeData &other) const
{
    return !(*this == other);
}

bool RideNodeData::operator<(const RideNodeData &other) const
{
    return (d->ride < other.ride());
}

bool RideNodeData::isNull() const
{
    return (d->ride.isNull() && d->stationList.isEmpty());
}

Ride RideNodeData::ride() const
{
    return d->ride;
}

void RideNodeData::setRide(const Ride &ride)
{
    d->ride = ride;
}

QList<Station> RideNodeData::stationList() const
{
    return d->stationList;
}

void RideNodeData::setStationList(const QList<Station> &stationList)
{
    d->stationList = stationList;
}

void RideNodeData::sort()
{
    std::sort(d->stationList.begin(), d->stationList.end());
}

}
