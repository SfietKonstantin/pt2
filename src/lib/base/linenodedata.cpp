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

#include "linenodedata.h"

/**
 * @file linenodedata.cpp
 * @short Implementation of PT2::LineNodeData
 */

namespace PT2
{

LineNodeDataPrivate::LineNodeDataPrivate()
    : QSharedData()
{
}

LineNodeDataPrivate::LineNodeDataPrivate(const LineNodeDataPrivate &other)
    : QSharedData(other), line(other.line), rideNodeDataList(other.rideNodeDataList)
{
}

LineNodeDataPrivate::~LineNodeDataPrivate()
{
}

////// End of private class //////

LineNodeData::LineNodeData(const Line &line, const QList<RideNodeData> &rideNodeDataList)
    : d(new LineNodeDataPrivate())
{
    d->line = line;
    d->rideNodeDataList = rideNodeDataList;
}

LineNodeData::LineNodeData(const LineNodeData &other)
    : d(other.d)
{
}

LineNodeData::~LineNodeData()
{
}

bool LineNodeData::operator==(const LineNodeData &other) const
{
    return (d->line == other.line() && d->rideNodeDataList == other.rideNodeDataList());
}

bool LineNodeData::operator!=(const LineNodeData &other) const
{
    return !(*this == other);
}

bool LineNodeData::operator<(const LineNodeData &other) const
{
    return (d->line < other.line());
}

bool LineNodeData::isNull() const
{
    return (d->line.isNull() && d->rideNodeDataList.isEmpty());
}

Line LineNodeData::line() const
{
    return d->line;
}

void LineNodeData::setLine(const Line &line)
{
    d->line = line;
}

QList<RideNodeData> LineNodeData::rideNodeDataList() const
{
    return d->rideNodeDataList;
}

void LineNodeData::setRideNodeDataList(const QList<RideNodeData> &rideNodeDataList)
{
    d->rideNodeDataList = rideNodeDataList;
}

void LineNodeData::sort()
{
    for (QList<RideNodeData>::iterator i = d->rideNodeDataList.begin(); i != d->rideNodeDataList.end(); ++i) {
        i->sort();
    }
    
std::sort(d->rideNodeDataList.begin(), d->rideNodeDataList.end());
}

}
