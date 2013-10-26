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

#ifndef PT2_ABSTRACTMODEL_H
#define PT2_ABSTRACTMODEL_H

/**
 * @file abstractmodel.h
 * @short Definition of PT2::AbstractModel
 */

#include <QtCore/QAbstractListModel>

namespace PT2
{

class AbstractModelPrivate;

/**
 * @brief Abstract model for PT2
 *
 * This abstract model is a base for all PT2
 * QML plugin models. It offers conveinent
 * base properties and methods.
 *
 * All models are tired to an PT2::AbstractBackendManager,
 * so this model offers the possibility to set one via
 * backendManager(). It also provides properties like
 * loading(), that tells if the backend is loading,
 * and count(), that gives the number of elements in the
 * model.
 *
 */
class AbstractModel : public QAbstractListModel
{
    Q_OBJECT
    /**
     * @short Loading
     */
    Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
    /**
     * @short Count
     */
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    /**
     * @brief Destructor
     */
    virtual ~AbstractModel();
    /**
     * @brief Is loading
     * @return if this model is loading.
     */
    bool isLoading() const;
    /**
     * @short Count
     * @return count.
     */
    int count() const;
    /**
     * @short Reimplementation of rowCount
     *
     * @param parent parent model index.
     * @return the number of rows in this model.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
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
     * @brief Clear
     */
    void clear();
Q_SIGNALS:
    /**
     * @brief Loading changed
     */
    void loadingChanged();
    /**
     * @short Count changed
     */
    void countChanged();
protected:
    /**
     * @brief D-pointer based constructor
     * @param dd D-pointer.
     * @param parent parent object.
     */
    explicit AbstractModel(AbstractModelPrivate &dd, QObject *parent = 0);
    /**
     * @short D-pointer
     */
    const QScopedPointer<AbstractModelPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(AbstractModel)
};

}

#endif // PT2_ABSTRACTMODEL_H
