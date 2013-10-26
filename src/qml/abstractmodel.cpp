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

/**
 * @file abstractmodel.cpp
 * @short Implementation of PT2::AbstractModel
 */

#include "abstractmodel.h"
#include "abstractmodel_p.h"
#include "manager/abstractbackendwrapper.h"
#include "manager/abstractbackendmanager.h"

namespace PT2
{

AbstractModelPrivate::AbstractModelPrivate(AbstractModel *q)
    : QObject(), q_ptr(q)
{
}

void AbstractModelPrivate::slotStatusChanged()
{
    AbstractBackendWrapper *backend = qobject_cast<AbstractBackendWrapper *>(sender());
    if (!backend) {
        return;
    }

    if (backend->status() == AbstractBackendWrapper::Launched) {
        connectBackend(backend);
    }

    if (backend->status() == AbstractBackendWrapper::Stopping) {
        disconnectBackend(backend);
    }
}

void AbstractModelPrivate::slotErrorRegistered(const QString &request, const QString &errorId,
                                               const QString &errorString)
{
    Q_UNUSED(errorId)
    Q_UNUSED(errorString)
    removeRequest(request);
}

void AbstractModelPrivate::addRequest(const QString &request)
{
    Q_Q(AbstractModel);
    bool empty = m_requests.isEmpty();
    m_requests.insert(request);
    if (empty) {
        emit q->loadingChanged();
    }

}

void AbstractModelPrivate::removeRequest(const QString &request)
{
    Q_Q(AbstractModel);
    m_requests.remove(request);
    if (m_requests.isEmpty()) {
        emit q->loadingChanged();
    }
}

bool AbstractModelPrivate::requestRunning(const QString &request)
{
    return m_requests.contains(request);
}

const ModelData & AbstractModelPrivate::data(int index) const
{
    return m_data.at(index);
}

void AbstractModelPrivate::addData(const ModelDataList &data)
{
    Q_Q(AbstractModel);
    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), q->rowCount(), q->rowCount() + data.count() - 1);
        m_data.append(data);
        emit q->countChanged();
        q->endInsertRows();
    }
}

void AbstractModelPrivate::connectBackend(AbstractBackendWrapper *backend)
{
    connect(backend, &AbstractBackendWrapper::errorRegistered,
            this, &AbstractModelPrivate::slotErrorRegistered);
}

void AbstractModelPrivate::disconnectBackend(AbstractBackendWrapper *backend)
{
    disconnect(backend, &AbstractBackendWrapper::errorRegistered,
               this, &AbstractModelPrivate::slotErrorRegistered);
}

////// End of private class //////

AbstractModel::AbstractModel(AbstractModelPrivate &dd, QObject *parent)
    : QAbstractListModel(parent), d_ptr(&dd)
{
}

AbstractModel::~AbstractModel()
{
}

bool AbstractModel::isLoading() const
{
    Q_D(const AbstractModel);
    return !d->m_requests.isEmpty();
}

int AbstractModel::count() const
{
    return rowCount();
}

int AbstractModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const AbstractModel);
    Q_UNUSED(parent)
    return d->m_data.count();
}

QVariant AbstractModel::data(const QModelIndex &index, int role) const
{
    Q_D(const AbstractModel);
    if (index.row() < 0 or index.row() >= rowCount()) {
        return QVariant();
    }

    return d->m_data.at(index.row()).value(role);
}

void AbstractModel::clear()
{
    Q_D(AbstractModel);
    d->m_requests.clear();
    if (d->m_data.count() > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        d->m_data.clear();
        endRemoveRows();
        emit countChanged();
    }
}

}
