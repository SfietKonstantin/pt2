#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
#
# You may use this file under the terms of the BSD license as follows:
#
# "Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#  *  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  *  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#  *  The names of its contributors may not be used to endorse or promote
#     products derived from this software without specific prior written
#     permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."

types = [{"node": "Ride", "data": "Station", "sort": False},
         {"node": "Line", "data": "RideNodeData"},
         {"node": "Company", "data": "LineNodeData"}]

copyright = """/*
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
"""

for type in types:
    node = type["node"]
    data = type["data"]
    sort = True
    if "sort" in type:
        sort = type["sort"]
    
    header = copyright
    header += """
#ifndef PT2___NODEu__NODEDATA_H
#define PT2___NODEu__NODEDATA_H

/**
 * @file __NODEl__nodedata.h
 * @short Definition of PT2::__NODE__NodeData
 */

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include \"__NODEl__.h\"
#include \"__DATAl__.h\"

namespace PT2
{

/**
 * @internal
 * @brief Private class for PT2::__NODE__NodeData
 *
 * This class is used to store private
 * data for PT2::__NODE__NodeData.
 */
class __NODE__NodeDataPrivate: public QSharedData
{
public:
    /**
     * @internal
     * @brief Default constructor
     */
    explicit __NODE__NodeDataPrivate();
    /**
     * @internal
     * @brief Copy constructor
     * @param other other object.
     */
    __NODE__NodeDataPrivate(const __NODE__NodeDataPrivate &other);
    /**
     * @internal
     * @brief Destructor
     */
    virtual ~__NODE__NodeDataPrivate();
    /**
     * @internal
     * @brief __NODE__
     */
    __NODE__ __NODEl__;
    /**
     * @internal
     * @brief __DATAcc__List
     */
    QList<__DATA__> __DATAcc__List;
};

/**
 * @internal
 * @brief A node with a __NODE__
 *
 * This class represents a node with a __NODE__.
 */
class __NODE__NodeData
{
public:
    /**
     * @short Default constructor
     *
     * @param __NODEl__ __NODEl__.
     * @param __DATAcc__List __DATAcc__List.
     */
    explicit __NODE__NodeData(const __NODE__ &__NODEl__ = __NODE__(), const QList<__DATA__> &__DATAcc__List = QList<__DATA__>());
    /**
     * @short Copy constructor
     *
     * @param other other object.
     */
    __NODE__NodeData(const __NODE__NodeData &other);
    /**
     * @short Equality
     *
     * @param other other object to compare to.
     * @return if the two __NODEl__ node data are equal.
     */
    bool operator==(const __NODE__NodeData &other) const;
    /**
     * @short Inequality
     *
     * @param other other object to compare to.
     * @return if the two __NODEl__ node data are not equal.
     */
    bool operator!=(const __NODE__NodeData &other) const;
    /**
     * @internal
     * @brief Inequality
     *
     * @param other other object to compare to.
     * @return if this __NODEl__ node is lesser that the other one.
     */
    bool operator<(const __NODE__NodeData &other) const;
    /**
     * @short Destructor
     */
    virtual ~__NODE__NodeData();
    /**
     * @brief If the __NODEl__ node data is null
     * @return if the __NODEl__ node data is null.
     */
    bool isNull() const;
    /**
     * @brief __NODE__
     * @return __NODEl__.
     */
    __NODE__ __NODEl__() const;
    /**
     * @brief Set __NODEl__
     * @param __NODEl__ __NODEl__ to set.
     */
    void set__NODE__(const __NODE__ &__NODEl__);
    /**
     * @brief Data contained in this node
     * @return data contained in this node.
     */
    QList<__DATA__> __DATAcc__List() const;
    /**
     * @brief Set the data contained in this node
     * @param __DATAcc__List data to set, as a list of __DATA__.
     */
    void set__DATA__List(const QList<__DATA__> &__DATAcc__List);
    /**
     * @short Sort the data contained in this node
     */
    void sort();
protected:
    /**
     * @brief D-pointer
     */
    QSharedDataPointer<__NODE__NodeDataPrivate> d;
};

}

Q_DECLARE_METATYPE(PT2::__NODE__NodeData)

#endif // PT2___NODEu__NODEDATA_H
"""
    header = header.replace("__NODEu__", node.upper())
    header = header.replace("__NODEl__", node.lower())
    header = header.replace("__NODE__", node)
    header = header.replace("__DATA__", data)
    header = header.replace("__DATAl__", data.lower())
    header = header.replace("__DATAcc__", data[0].lower() + data[1:])
    f = open(node.lower() + "nodedata.h", "w")
    f.write(header)
    f.close()
    
    source = copyright
    source += """
#include "__NODEl__nodedata.h"

/**
 * @file __NODEl__nodedata.cpp
 * @short Implementation of PT2::__NODE__NodeData
 */

namespace PT2
{

__NODE__NodeDataPrivate::__NODE__NodeDataPrivate()
    : QSharedData()
{
}

__NODE__NodeDataPrivate::__NODE__NodeDataPrivate(const __NODE__NodeDataPrivate &other)
    : QSharedData(other), __NODEl__(other.__NODEl__), __DATAcc__List(other.__DATAcc__List)
{
}

__NODE__NodeDataPrivate::~__NODE__NodeDataPrivate()
{
}

////// End of private class //////

__NODE__NodeData::__NODE__NodeData(const __NODE__ &__NODEl__, const QList<__DATA__> &__DATAcc__List)
    : d(new __NODE__NodeDataPrivate())
{
    d->__NODEl__ = __NODEl__;
    d->__DATAcc__List = __DATAcc__List;
}

__NODE__NodeData::__NODE__NodeData(const __NODE__NodeData &other)
    : d(other.d)
{
}

__NODE__NodeData::~__NODE__NodeData()
{
}

bool __NODE__NodeData::operator==(const __NODE__NodeData &other) const
{
    return (d->__NODEl__ == other.__NODEl__() && d->__DATAcc__List == other.__DATAcc__List());
}

bool __NODE__NodeData::operator!=(const __NODE__NodeData &other) const
{
    return !(*this == other);
}

bool __NODE__NodeData::operator<(const __NODE__NodeData &other) const
{
    return (d->__NODEl__ < other.__NODEl__());
}

bool __NODE__NodeData::isNull() const
{
    return (d->__NODEl__.isNull() && d->__DATAcc__List.isEmpty());
}

__NODE__ __NODE__NodeData::__NODEl__() const
{
    return d->__NODEl__;
}

void __NODE__NodeData::set__NODE__(const __NODE__ &__NODEl__)
{
    d->__NODEl__ = __NODEl__;
}

QList<__DATA__> __NODE__NodeData::__DATAcc__List() const
{
    return d->__DATAcc__List;
}

void __NODE__NodeData::set__DATA__List(const QList<__DATA__> &__DATAcc__List)
{
    d->__DATAcc__List = __DATAcc__List;
}

void __NODE__NodeData::sort()
{
    __SORT__std::sort(d->__DATAcc__List.begin(), d->__DATAcc__List.end());
}

}
"""
    if sort:
        source = source.replace("__SORT__", """for (QList<__DATA__>::iterator i = d->__DATAcc__List.begin(); i != d->__DATAcc__List.end(); ++i) {
        i->sort();
    }
    
""")
    else:
        source = source.replace("__SORT__", "")

    source = source.replace("__NODEu__", node.upper())
    source = source.replace("__NODEl__", node.lower())
    source = source.replace("__NODE__", node)
    source = source.replace("__DATA__", data)
    source = source.replace("__DATAl__", data.lower())
    source = source.replace("__DATAcc__", data[0].lower() + data[1:])
    f = open(node.lower() + "nodedata.cpp", "w")
    f.write(source)
    f.close()


    
