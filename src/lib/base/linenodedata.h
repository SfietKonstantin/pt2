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

#ifndef PT2_LINENODEDATA_H
#define PT2_LINENODEDATA_H

/**
 * @file linenodedata.h
 * @short Definition of PT2::LineNodeData
 */

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include "line.h"
#include "ridenodedata.h"

namespace PT2
{

/**
 * @internal
 * @brief Private class for PT2::LineNodeData
 *
 * This class is used to store private
 * data for PT2::LineNodeData.
 */
class LineNodeDataPrivate: public QSharedData
{
public:
    /**
     * @internal
     * @brief Default constructor
     */
    explicit LineNodeDataPrivate();
    /**
     * @internal
     * @brief Copy constructor
     * @param other other object.
     */
    LineNodeDataPrivate(const LineNodeDataPrivate &other);
    /**
     * @internal
     * @brief Destructor
     */
    virtual ~LineNodeDataPrivate();
    /**
     * @internal
     * @brief Line
     */
    Line line;
    /**
     * @internal
     * @brief rideNodeDataList
     */
    QList<RideNodeData> rideNodeDataList;
};

/**
 * @internal
 * @brief A node with a Line
 *
 * This class represents a node with a Line.
 */
class LineNodeData
{
public:
    /**
     * @short Default constructor
     *
     * @param line line.
     * @param rideNodeDataList rideNodeDataList.
     */
    explicit LineNodeData(const Line &line = Line(), const QList<RideNodeData> &rideNodeDataList = QList<RideNodeData>());
    /**
     * @short Copy constructor
     *
     * @param other other object.
     */
    LineNodeData(const LineNodeData &other);
    /**
     * @short Equality
     *
     * @param other other object to compare to.
     * @return if the two line node data are equal.
     */
    bool operator==(const LineNodeData &other) const;
    /**
     * @short Inequality
     *
     * @param other other object to compare to.
     * @return if the two line node data are not equal.
     */
    bool operator!=(const LineNodeData &other) const;
    /**
     * @internal
     * @brief Inequality
     *
     * @param other other object to compare to.
     * @return if this line node is lesser that the other one.
     */
    bool operator<(const LineNodeData &other) const;
    /**
     * @short Destructor
     */
    virtual ~LineNodeData();
    /**
     * @brief If the line node data is null
     * @return if the line node data is null.
     */
    bool isNull() const;
    /**
     * @brief Line
     * @return line.
     */
    Line line() const;
    /**
     * @brief Set line
     * @param line line to set.
     */
    void setLine(const Line &line);
    /**
     * @brief Data contained in this node
     * @return data contained in this node.
     */
    QList<RideNodeData> rideNodeDataList() const;
    /**
     * @brief Set the data contained in this node
     * @param rideNodeDataList data to set, as a list of RideNodeData.
     */
    void setRideNodeDataList(const QList<RideNodeData> &rideNodeDataList);
    /**
     * @short Sort the data contained in this node
     */
    void sort();
protected:
    /**
     * @brief D-pointer
     */
    QSharedDataPointer<LineNodeDataPrivate> d;
};

}

Q_DECLARE_METATYPE(PT2::LineNodeData)

#endif // PT2_LINENODEDATA_H
