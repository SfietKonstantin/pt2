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
 * @file backendmodel.cpp
 * @short Implementation of PT2::BackendModel
 */

#include "backendmodel.h"

#include "manager/backendlistmanager.h"
#include "manager/abstractbackendmanager.h"
#include "manager/backendinfo.h"
#include "debug.h"

namespace PT2
{

/**
 * @internal
 * @short Private class for PT2::BackendModel
 */
class BackendModelPrivate: public QObject
{
    Q_OBJECT
public:
    /**
     * @internal
     * @short Default constructor
     * @param q Q-pointer.
     */
    BackendModelPrivate(BackendModel *q);
    /**
     * @internal
     * @brief Status
     * @param backendInfo backend to get the status.
     * @return status of the backend.
     */
    BackendModel::BackendStatus status(const BackendInfo &backendInfo) const;
    /**
     * @internal
     * @brief Apply filter
     */
    void applyfilter();
    /**
     * @internal
     * @brief Filter
     */
    QString filter;
    /**
     * @internal
     * @brief Backend list manager
     */
    BackendListManager *backendListManager;
    /**
     * @internal
     * @brief Backend manager
     */
    AbstractBackendManager *backendManager;
    /**
     * @internal
     * @short Backends
     */
    QList<BackendInfo> backends;
    /**
     * @internal
     * @short Data
     */
    QList<BackendInfo> data;
public Q_SLOTS:
    /**
     * @internal
     * @brief Slot for status changed
     */
    void slotStatusChanged();
private:
    /**
     * @internal
     * @short Q-pointer
     */
    BackendModel * const q_ptr;
    Q_DECLARE_PUBLIC(BackendModel)
};

BackendModelPrivate::BackendModelPrivate(BackendModel *q):
    q_ptr(q)
{
    backendManager = 0;
    backendListManager = new BackendListManager(q);
}

BackendModel::BackendStatus BackendModelPrivate::status(const BackendInfo &backendInfo) const
{
    QString identifier = backendInfo.backendIdentifier();

    if (!backendManager->contains(identifier)) {
        return BackendModel::Stopped;
    }

    AbstractBackendWrapper *backendWrapper = backendManager->backend(identifier);
    return (BackendModel::BackendStatus) backendWrapper->status();
}

void BackendModelPrivate::applyfilter()
{
    Q_Q(BackendModel);
    if (data.count() > 0) {
        q->beginRemoveRows(QModelIndex(), 0, data.count() - 1);
        data.clear();
        q->endRemoveRows();
    }

    foreach (BackendInfo backendInfo, backends) {
        if (filter.isEmpty() || backendInfo.backendCountry() == filter) {
            data.append(backendInfo);
        }
    }

    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), 0, data.count() - 1);
        emit q->countChanged();
        q->endInsertRows();
    }
}

void BackendModelPrivate::slotStatusChanged()
{
    Q_Q(BackendModel);
    AbstractBackendWrapper *backendWrapper = qobject_cast<AbstractBackendWrapper *>(sender());
    if (!backendWrapper) {
        return;
    }

    if (!backendManager) {
        return;
    }

    QString identifier = backendWrapper->identifier();

    int index = -1;
    for (int i = 0; i < data.size(); i++) {
        if (backends.at(i).backendIdentifier() == identifier) {
            index = i;
        }
    }

    if (index == -1) {
        return;
    }

    emit q->dataChanged(q->index(index), q->index(index));
}


////// End of private class //////

BackendModel::BackendModel(QObject *parent):
    QAbstractListModel(parent),
    d_ptr(new BackendModelPrivate(this))
{
}

BackendModel::~BackendModel()
{
}

QHash<int, QByteArray> BackendModel::roleNames() const
{
    // Definition of roles
    QHash <int, QByteArray> roles;
    roles.insert(NameRole, "name");
    roles.insert(DescriptionRole, "description");
    roles.insert(StatusRole, "status");
    roles.insert(IdentifierRole, "identifier");
    return roles;
}

AbstractBackendManager * BackendModel::backendManager() const
{
    Q_D(const BackendModel);
    return d->backendManager;
}

void BackendModel::setBackendManager(AbstractBackendManager *backendManager)
{
    Q_D(BackendModel);
    if (d->backendManager != backendManager) {
        d->backendManager = backendManager;
        emit backendManagerChanged();
    }
}

int  BackendModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    Q_D(const BackendModel);
    return d->data.count();
}

QString BackendModel::filter() const
{
    Q_D(const BackendModel);
    return d->filter;
}

void BackendModel::setFilter(const QString &filter)
{
    Q_D(BackendModel);
    if (d->filter != filter) {
        d->filter = filter;
        d->applyfilter();
    }
}

int BackendModel::count() const
{
    return rowCount();
}

QVariant BackendModel::data(const QModelIndex &index, int role) const
{
    Q_D(const BackendModel);
    if (index.row() < 0 or index.row() >= rowCount()) {
        return QVariant();
    }
    BackendInfo backendInfo = d->data.at(index.row());

    switch(role) {
    case NameRole:
        return backendInfo.name();
        break;
    case DescriptionRole:
        return backendInfo.description();
        break;
    case StatusRole:
        return d->status(backendInfo);
        break;
    case IdentifierRole:
        return backendInfo.backendIdentifier();
        break;
    default:
        return QVariant();
        break;
    }
}

void BackendModel::reload()
{
    Q_D(BackendModel);
    d->backendListManager->reload();
    QList<BackendInfo> availableBackendList = d->backendListManager->backendList();
    d->backends = availableBackendList;
    d->applyfilter();

    debug("backend-model") << "Reloaded information in the model. Number of rows:" << rowCount();
}

void BackendModel::startBackend(const QString &identifier)
{
    Q_D(BackendModel);
    if (!d->backendManager) {
        return;
    }

    if (d->backendManager->contains(identifier)) {
        AbstractBackendWrapper *backend = d->backendManager->backend(identifier);

        if (backend->status() == AbstractBackendWrapper::Stopped) {
            backend->launch();
            return;
        } else {
            backend->kill();
            backend->launch();
            return;
        }
    }

    QMap<QString, BackendInfo> identifierToBackend;
    foreach (BackendInfo backendInfo, d->backendListManager->backendList()) {
        identifierToBackend.insert(backendInfo.backendIdentifier(), backendInfo);
    }

    d->backendManager->addBackend(identifier, identifierToBackend.value(identifier).executable(),
                                  QMap<QString, QString>());

    connect(d->backendManager->backend(identifier), &AbstractBackendWrapper::statusChanged,
            d, &BackendModelPrivate::slotStatusChanged);

    d->backendManager->launchBackend(identifier);
}

void BackendModel::stopBackend(const QString &identifier)
{
    Q_D(BackendModel);
    if (!d->backendManager) {
        return;
    }

    if (!d->backendManager->contains(identifier)) {
        return;
    }

    d->backendManager->stopBackend(identifier);
}

}

#include "backendmodel.moc"
