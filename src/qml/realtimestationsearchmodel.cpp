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
 * @file realtimestationsearchmodel.cpp
 * @short Implementation of PT2::RealTimeStationSearchModel
 */

#include "realtimestationsearchmodel.h"
#include "abstractmultibackendmodel_p.h"

#include "capabilitiesconstants.h"
#include "base/station.h"
#include "manager/abstractbackendmanager.h"
#include "manager/abstractbackendwrapper.h"
#include "debug.h"

namespace PT2
{

static int STATION_INDEX = 10;
static int BACKEND_IDENTIFIER_INDEX = 11;

/**
 * @internal
 * @short Private class for PT2::RealTimeStationSearchModel
 */
class RealTimeStationSearchModelPrivate: public AbstractMultiBackendModelPrivate
{
    Q_OBJECT
public:
    /**
     * @internal
     * @short Default constructor
     * @param q Q-pointer.
     */
    explicit RealTimeStationSearchModelPrivate(RealTimeStationSearchModel *q);
    void setShort(bool isShort);
protected:
    void connectBackend(AbstractBackendWrapper *backend);
    void disconnectBackend(AbstractBackendWrapper *backend);
public Q_SLOTS:
    /**
     * @internal
     * @brief Slot suggested stations registered
     * @param request request identifier.
     * @param stations stations.
     */
    void slotStationsRegistered(const QString & request, const QList<PT2::Station> &stations);
private:
    bool m_short;
    Q_DECLARE_PUBLIC(RealTimeStationSearchModel)
};

RealTimeStationSearchModelPrivate::RealTimeStationSearchModelPrivate(RealTimeStationSearchModel *q):
    AbstractMultiBackendModelPrivate(q), m_short(false)
{
}

void RealTimeStationSearchModelPrivate::setShort(bool isShort)
{
    Q_Q(RealTimeStationSearchModel);
    if (m_short != isShort) {
        m_short = isShort;
        emit q->shortChanged();
    }
}

void RealTimeStationSearchModelPrivate::connectBackend(AbstractBackendWrapper *backend)
{
    connect(backend, &AbstractBackendWrapper::realTimeSuggestedStationsRegistered,
            this, &RealTimeStationSearchModelPrivate::slotStationsRegistered);
    AbstractMultiBackendModelPrivate::connectBackend(backend);
}

void RealTimeStationSearchModelPrivate::disconnectBackend(AbstractBackendWrapper *backend)
{
    disconnect(backend, &AbstractBackendWrapper::realTimeSuggestedStationsRegistered,
               this, &RealTimeStationSearchModelPrivate::slotStationsRegistered);
    AbstractMultiBackendModelPrivate::disconnectBackend(backend);
}

void RealTimeStationSearchModelPrivate::slotStationsRegistered(const QString & request,
                                                               const QList<Station> &stations)
{
    if (!requestRunning(request)) {
        return;
    }

    AbstractBackendWrapper *backend = qobject_cast<AbstractBackendWrapper *>(sender());
    if (!backend) {
        return;
    }

    debug("realtime-station-search-model") << "Request" << request << "finished";

    removeRequest(request);

    ModelDataList addedData;
    foreach (Station station, stations) {
        ModelData data;
        data.insert(Qt::UserRole + STATION_INDEX, QVariant::fromValue(station));
        data.insert(Qt::UserRole + BACKEND_IDENTIFIER_INDEX, backend->identifier());
        data.insert(RealTimeStationSearchModel::NameRole, station.name());
        // TODO ProviderNameRole
        addedData.append(data);
    }
    debug("station-search-model") << "Inserting" << addedData.count() << "elements";
    addData(addedData);
}


////// End of private class //////

RealTimeStationSearchModel::RealTimeStationSearchModel(QObject *parent)
    : AbstractMultiBackendModel(*(new RealTimeStationSearchModelPrivate(this)), parent)
{
}

bool RealTimeStationSearchModel::isShort() const
{
    Q_D(const RealTimeStationSearchModel);
    return d->m_short;
}

QHash<int, QByteArray> RealTimeStationSearchModel::roleNames() const
{
    QHash <int, QByteArray> roles;
    roles.insert(NameRole, "name");
    roles.insert(ProviderNameRole, "providerName");
    return roles;
}

void RealTimeStationSearchModel::search(const QString &partialStation)
{
    Q_D(RealTimeStationSearchModel);

    clear();

    d->setShort(false);
    QString partialStationTrimmed = partialStation.trimmed();
    if (partialStationTrimmed.count() < 3) {
        d->setShort(true);
        return;
    }

    foreach (AbstractBackendWrapper *backend, d->backendManager->backends()) {
        if (backend->capabilities().contains(CAPABILITY_REAL_TIME_SUGGEST_STATION_FROM_STRING)) {
            QString request = backend->requestRealTimeSuggestedStations(partialStationTrimmed);
            d->addRequest(request);
        }
    }
}

}

#include "realtimestationsearchmodel.moc"
