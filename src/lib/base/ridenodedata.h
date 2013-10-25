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

#ifndef PT2_RIDENODEDATA_H
#define PT2_RIDENODEDATA_H

/**
 * @file ridenodedata.h
 * @short Definition of PT2::RideNodeData
 */

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include "ride.h"
#include "station.h"

namespace PT2
{

/**
 * @internal
 * @brief Private class for PT2::RideNodeData
 *
 * This class is used to store private
 * data for PT2::RideNodeData.
 */
class RideNodeDataPrivate: public QSharedData
{
public:
    /**
     * @internal
     * @brief Default constructor
     */
    explicit RideNodeDataPrivate();
    /**
     * @internal
     * @brief Copy constructor
     * @param other other object.
     */
    RideNodeDataPrivate(const RideNodeDataPrivate &other);
    /**
     * @internal
     * @brief Destructor
     */
    virtual ~RideNodeDataPrivate();
    /**
     * @internal
     * @brief Ride
     */
    Ride ride;
    /**
     * @internal
     * @brief stationList
     */
    QList<Station> stationList;
};

/**
 * @internal
 * @brief A node with a Ride
 *
 * This class represents a node with a Ride.
 */
class RideNodeData
{
public:
    /**
     * @short Default constructor
     *
     * @param ride ride.
     * @param stationList stationList.
     */
    explicit RideNodeData(const Ride &ride = Ride(), const QList<Station> &stationList = QList<Station>());
    /**
     * @short Copy constructor
     *
     * @param other other object.
     */
    RideNodeData(const RideNodeData &other);
    /**
     * @short Equality
     *
     * @param other other object to compare to.
     * @return if the two ride node data are equal.
     */
    bool operator==(const RideNodeData &other) const;
    /**
     * @short Inequality
     *
     * @param other other object to compare to.
     * @return if the two ride node data are not equal.
     */
    bool operator!=(const RideNodeData &other) const;
    /**
     * @internal
     * @brief Inequality
     *
     * @param other other object to compare to.
     * @return if this ride node is lesser that the other one.
     */
    bool operator<(const RideNodeData &other) const;
    /**
     * @short Destructor
     */
    virtual ~RideNodeData();
    /**
     * @brief If the ride node data is null
     * @return if the ride node data is null.
     */
    bool isNull() const;
    /**
     * @brief Ride
     * @return ride.
     */
    Ride ride() const;
    /**
     * @brief Set ride
     * @param ride ride to set.
     */
    void setRide(const Ride &ride);
    /**
     * @brief Data contained in this node
     * @return data contained in this node.
     */
    QList<Station> stationList() const;
    /**
     * @brief Set the data contained in this node
     * @param stationList data to set, as a list of Station.
     */
    void setStationList(const QList<Station> &stationList);
    /**
     * @short Sort the data contained in this node
     */
    void sort();
protected:
    /**
     * @brief D-pointer
     */
    QSharedDataPointer<RideNodeDataPrivate> d;
};

}

Q_DECLARE_METATYPE(PT2::RideNodeData)

#endif // PT2_RIDENODEDATA_H
