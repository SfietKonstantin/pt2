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

#ifndef PT2_ABSTRACTMODEL_P_H
#define PT2_ABSTRACTMODEL_P_H

/**
 * @file abstractmodel_p.h
 * @short Definition of PT2::AbstractModelPrivate
 */

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QSet>

namespace PT2
{

/**
 * @internal
 * @brief Model data
 *
 * Used in PT2::AbstractModelPrivate.
 */
typedef QMap<int, QVariant> ModelData;
/**
 * @internal
 * @brief Model data list
 *
 * Used in PT2::AbstractModelPrivate.
 */
typedef QList<ModelData> ModelDataList;

class AbstractBackendWrapper;
class AbstractModel;
/**
 * @internal
 * @short Private class for PT2::AbstractModel
 */
class AbstractModelPrivate: public QObject
{
    Q_OBJECT
public:
    /**
     * @internal
     * @short Default constructor
     * @param q Q-pointer.
     */
    explicit AbstractModelPrivate(AbstractModel *q);
public Q_SLOTS:
    /**
     * @internal
     * @brief Slot status changed
     */
    void slotStatusChanged();
    /**
     * @internal
     * @brief Slot error registered
     * @param request request identifier.
     * @param errorId error identifier.
     * @param errorString error string.
     */
    void slotErrorRegistered(const QString &request, const QString &errorId,
                             const QString &errorString);
protected:
    /**
     * @internal
     * @brief Add a request
     * @param request request to add.
     */
    void addRequest(const QString &request);
    /**
     * @internal
     * @brief Remove a request
     * @param request request to remove.
     */
    void removeRequest(const QString &request);
    /**
     * @internal
     * @brief If the model is running the request
     * @param request request.
     * @return if the model is running the request.
     */
    bool requestRunning(const QString &request);
    const ModelData & data(int index) const;
    /**
     * @internal
     * @brief Add data
     * @param data data to add.
     */
    void addData(const ModelDataList &data);
    /**
     * @internal
     * @brief Connect backend
     *
     * Connect signals from backend to this class.
     *
     * @param backend backend to connect.
     */
    virtual void connectBackend(AbstractBackendWrapper *backend);
    /**
     * @internal
     * @brief Disconnect backend
     *
     * Disconnect signals from backend to this class.
     *
     * @param backend backend to disconnect.
     */
    virtual void disconnectBackend(AbstractBackendWrapper *backend);
    /**
     * @internal
     * @short Q-pointer
     */
    AbstractModel * const q_ptr;
private:
    /**
     * @internal
     * @brief Data
     */
    ModelDataList m_data;
    /**
     * @internal
     * @brief Requests
     */
    QSet<QString> m_requests;
    Q_DECLARE_PUBLIC(AbstractModel)
};


}

#endif // PT2_ABSTRACTMODEL_P_H
