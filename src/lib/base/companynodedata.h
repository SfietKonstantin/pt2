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

#ifndef PT2_COMPANYNODEDATA_H
#define PT2_COMPANYNODEDATA_H

/**
 * @file companynodedata.h
 * @short Definition of PT2::CompanyNodeData
 */

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include "company.h"
#include "linenodedata.h"

namespace PT2
{

/**
 * @internal
 * @brief Private class for PT2::CompanyNodeData
 *
 * This class is used to store private
 * data for PT2::CompanyNodeData.
 */
class CompanyNodeDataPrivate: public QSharedData
{
public:
    /**
     * @internal
     * @brief Default constructor
     */
    explicit CompanyNodeDataPrivate();
    /**
     * @internal
     * @brief Copy constructor
     * @param other other object.
     */
    CompanyNodeDataPrivate(const CompanyNodeDataPrivate &other);
    /**
     * @internal
     * @brief Destructor
     */
    virtual ~CompanyNodeDataPrivate();
    /**
     * @internal
     * @brief Company
     */
    Company company;
    /**
     * @internal
     * @brief lineNodeDataList
     */
    QList<LineNodeData> lineNodeDataList;
};

/**
 * @internal
 * @brief A node with a Company
 *
 * This class represents a node with a Company.
 */
class CompanyNodeData
{
public:
    /**
     * @short Default constructor
     *
     * @param company company.
     * @param lineNodeDataList lineNodeDataList.
     */
    explicit CompanyNodeData(const Company &company = Company(), const QList<LineNodeData> &lineNodeDataList = QList<LineNodeData>());
    /**
     * @short Copy constructor
     *
     * @param other other object.
     */
    CompanyNodeData(const CompanyNodeData &other);
    /**
     * @short Equality
     *
     * @param other other object to compare to.
     * @return if the two company node data are equal.
     */
    bool operator==(const CompanyNodeData &other) const;
    /**
     * @short Inequality
     *
     * @param other other object to compare to.
     * @return if the two company node data are not equal.
     */
    bool operator!=(const CompanyNodeData &other) const;
    /**
     * @internal
     * @brief Inequality
     *
     * @param other other object to compare to.
     * @return if this company node is lesser that the other one.
     */
    bool operator<(const CompanyNodeData &other) const;
    /**
     * @short Destructor
     */
    virtual ~CompanyNodeData();
    /**
     * @brief If the company node data is null
     * @return if the company node data is null.
     */
    bool isNull() const;
    /**
     * @brief Company
     * @return company.
     */
    Company company() const;
    /**
     * @brief Set company
     * @param company company to set.
     */
    void setCompany(const Company &company);
    /**
     * @brief Data contained in this node
     * @return data contained in this node.
     */
    QList<LineNodeData> lineNodeDataList() const;
    /**
     * @brief Set the data contained in this node
     * @param lineNodeDataList data to set, as a list of LineNodeData.
     */
    void setLineNodeDataList(const QList<LineNodeData> &lineNodeDataList);
    /**
     * @short Sort the data contained in this node
     */
    void sort();
protected:
    /**
     * @brief D-pointer
     */
    QSharedDataPointer<CompanyNodeDataPrivate> d;
};

}

Q_DECLARE_METATYPE(PT2::CompanyNodeData)

#endif // PT2_COMPANYNODEDATA_H
