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

#include "companynodedata.h"

/**
 * @file companynodedata.cpp
 * @short Implementation of PT2::CompanyNodeData
 */

namespace PT2
{

CompanyNodeDataPrivate::CompanyNodeDataPrivate()
    : QSharedData()
{
}

CompanyNodeDataPrivate::CompanyNodeDataPrivate(const CompanyNodeDataPrivate &other)
    : QSharedData(other), company(other.company), lineNodeDataList(other.lineNodeDataList)
{
}

CompanyNodeDataPrivate::~CompanyNodeDataPrivate()
{
}

////// End of private class //////

CompanyNodeData::CompanyNodeData(const Company &company, const QList<LineNodeData> &lineNodeDataList)
    : d(new CompanyNodeDataPrivate())
{
    d->company = company;
    d->lineNodeDataList = lineNodeDataList;
}

CompanyNodeData::CompanyNodeData(const CompanyNodeData &other)
    : d(other.d)
{
}

CompanyNodeData::~CompanyNodeData()
{
}

bool CompanyNodeData::operator==(const CompanyNodeData &other) const
{
    return (d->company == other.company() && d->lineNodeDataList == other.lineNodeDataList());
}

bool CompanyNodeData::operator!=(const CompanyNodeData &other) const
{
    return !(*this == other);
}

bool CompanyNodeData::operator<(const CompanyNodeData &other) const
{
    return (d->company < other.company());
}

bool CompanyNodeData::isNull() const
{
    return (d->company.isNull() && d->lineNodeDataList.isEmpty());
}

Company CompanyNodeData::company() const
{
    return d->company;
}

void CompanyNodeData::setCompany(const Company &company)
{
    d->company = company;
}

QList<LineNodeData> CompanyNodeData::lineNodeDataList() const
{
    return d->lineNodeDataList;
}

void CompanyNodeData::setLineNodeDataList(const QList<LineNodeData> &lineNodeDataList)
{
    d->lineNodeDataList = lineNodeDataList;
}

void CompanyNodeData::sort()
{
    for (QList<LineNodeData>::iterator i = d->lineNodeDataList.begin(); i != d->lineNodeDataList.end(); ++i) {
        i->sort();
    }
    
std::sort(d->lineNodeDataList.begin(), d->lineNodeDataList.end());
}

}
