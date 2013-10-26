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

#ifndef PT2_BACKENDMODEL_H
#define PT2_BACKENDMODEL_H

/**
 * @file backendmodel.h
 * @short Definition of PT2::BackendModel
 */

#include <QtCore/QAbstractListModel>
#include "manager/abstractbackendwrapper.h"

namespace PT2
{

class AbstractBackendManager;
class BackendModelPrivate;
/**
 * @brief A model for available backends
 *
 * This class provides a model for QML that contains
 * the available backends. It also provides control
 * over the backends, as well as feedback on backend
 * status.
 *
 * It mostly mirrors PublicTransportation::AbstractBackendManager,
 * adding only a feature, that is starting previously started
 * backend when reloading the backend list.
 *
 * When a backend have been previously started, and not
 * stopped, it is considered as useful, and should be
 * restart. When the list of backends are reloaded, these
 * backends are then restarted automatically (that happen,
 * for example, while the application is launched again).
 *
 * This model can also be filtered, using the filter() property.
 * This filter takes a country code, or an empty string for all countries,
 * and will filter the list by countries.
 */
class BackendModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(BackendStatus)
    /**
     * @short Filter
     */
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    /**
     * @short Count
     */
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    /**
     * @short Backend manager
     */
    Q_PROPERTY(PT2::AbstractBackendManager * backendManager READ backendManager
               WRITE setBackendManager NOTIFY backendManagerChanged)
public:
    /**
     * @brief Enumeration describing backend status
     */
    enum BackendStatus {
        /**
         * @short The backend is stopped
         */
        Stopped = AbstractBackendWrapper::Stopped,
        /**
         * @short The backend is launching
         */
        Launching = AbstractBackendWrapper::Launching,
        /**
         * @short The backend is launched
         */
        Launched = AbstractBackendWrapper::Launched,
        /**
         * @short The backend is stopping
         */
        Stopping = AbstractBackendWrapper::Stopping,
        /**
         * @short The backend is in an invalid state
         */
        Invalid = AbstractBackendWrapper::Invalid
    };

    /**
     * @short Model roles
     */
    enum BackendModelRole {
        /**
         * @short Name role
         */
        NameRole = Qt::UserRole + 1,
        /**
         * @short Description role
         */
        DescriptionRole,
        /**
         * @short Status role
         */
        StatusRole,
        /**
         * @short Identifier role
         */
        IdentifierRole
    };
    /**
     * @short Default constructor
     * @param parent parent object.
     */
    explicit BackendModel(QObject *parent = 0);
    /**
     * @short Destructor
     */
    virtual ~BackendModel();
    /**
     * @short Role names
     * @return role names.
     */
    QHash<int, QByteArray> roleNames() const;
    /**
     * @brief Backend manager
     * @return backend manager.
     */
    AbstractBackendManager * backendManager() const;
    /**
     * @brief Set backend manager
     * @param backendManager backend manager to set.
     */
    void setBackendManager(AbstractBackendManager *backendManager);
    /**
     * @short Reimplementation of rowCount
     *
     * @param parent parent model index.
     * @return the number of rows in this model.
     */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    /**
     * @brief Filter
     * @return filter.
     */
    QString filter() const;
    /**
     * @brief Set filter
     * @param filter filter to set.
     */
    void setFilter(const QString &filter);
    /**
     * @short Count
     * @return count.
     */
    int count() const;
    /**
     * @short Reimplementation of data
     *
     * @param index model index where retrieve the data.
     * @param role role to retrieve.
     * @return retrieved data as a variant.
     */
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
public Q_SLOTS:
    /**
     * @brief Reload
     *
     * Reload the backend list.
     */
    void reload();
    /**
     * @brief Start a backend
     * @param identifier backend identifier.
     */
    void startBackend(const QString &identifier);
    /**
     * @brief Stop a backend.
     * @param identifier backend identifier.
     */
    void stopBackend(const QString &identifier);
Q_SIGNALS:
    /**
     * @brief Filter changed
     */
    void filterChanged();
    /**
     * @short Count changed
     */
    void countChanged();
    /**
     * @brief Backend manager changed
     */
    void backendManagerChanged();
protected:
    /**
     * @short D-pointer
     */
    const QScopedPointer<BackendModelPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(BackendModel)

};

}

#endif // PT2_BACKENDMODEL_H
