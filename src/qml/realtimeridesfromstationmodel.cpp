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

#include "realtimeridesfromstationmodel.h"
#include "abstractmodel_p.h"
#include "realtimestationsearchmodel.h"
#include "debug.h"
#include "base/station.h"
#include "base/ride.h"
#include "base/companynodedata.h"
#include "manager/abstractbackendwrapper.h"

namespace PT2
{

class RealTimeRidesFromStationModelPrivate: public AbstractModelPrivate
{
    Q_OBJECT
public:
    explicit RealTimeRidesFromStationModelPrivate(RealTimeRidesFromStationModel *q);
    RealTimeStationSearchModel * stationSearchModel;
    Station currentStation;
public Q_SLOTS:
    void slotRidesFromStationRequested(AbstractBackendWrapper *backend, const QString &request,
                                       const Station &station);
    /**
     * @internal
     * @brief Slot rides from stations registered
     * @param request request identifier.
     * @param rides rides.
     */
    void slotRidesFromStationRegistered(const QString &request, const QList<CompanyNodeData> &rides);
protected:
    void connectBackend(AbstractBackendWrapper *backend);
    void disconnectBackend(AbstractBackendWrapper *backend);
};

RealTimeRidesFromStationModelPrivate::RealTimeRidesFromStationModelPrivate(RealTimeRidesFromStationModel *q)
    : AbstractModelPrivate(q), stationSearchModel(0)
{
}

void RealTimeRidesFromStationModelPrivate::slotRidesFromStationRequested(AbstractBackendWrapper *backend,
                                                                         const QString &request,
                                                                         const Station &station)
{
    connectBackend(backend);
    addRequest(request);
    currentStation = station;
}

void RealTimeRidesFromStationModelPrivate::slotRidesFromStationRegistered(const QString &request,
                                                                          const QList<CompanyNodeData> &rides)
{
    if (!requestRunning(request)) {
        return;
    }

    AbstractBackendWrapper *backend = qobject_cast<AbstractBackendWrapper *>(sender());
    if (!backend) {
        return;
    }

    ModelDataList dataList;

    debug("realtime-rides-from-station-model") << "Request" << request << "finished";
    debug("realtime-rides-from-station-model") << "Received" << rides.count() << "root data";
    foreach (CompanyNodeData companyNodeData, rides) {
        companyNodeData.sort();
        debug("realtime-rides-from-station-model") << "Received"
                                                   << companyNodeData.lineNodeDataList().count()
                                                   << "line data";

        foreach (const LineNodeData &lineNodeData, companyNodeData.lineNodeDataList()) {
            QString line = lineNodeData.line().name();

            foreach (const RideNodeData &rideNodeData, lineNodeData.rideNodeDataList()) {
                if (rideNodeData.stationList().count() != 1) {
                    warning("realtime-rides-from-station-model") << "The number of stations "\
                                                                    "should be 1.";
                    removeRequest(request);
                    return;
                }

                QString ride = rideNodeData.ride().name();
                ModelData data;
                data.insert(RealTimeRidesFromStationModel::LineRole, line);
                data.insert(RealTimeRidesFromStationModel::NameRole, ride);
                dataList.append(data);
            }
        }
    }

    debug("realtime-rides-from-station-model") << "Displaying" << dataList.count() << "entries";

    addData(dataList);
    removeRequest(request);
}

void RealTimeRidesFromStationModelPrivate::connectBackend(AbstractBackendWrapper *backend)
{
    connect(backend, &AbstractBackendWrapper::realTimeRidesFromStationRegistered,
            this, &RealTimeRidesFromStationModelPrivate::slotRidesFromStationRegistered);
    AbstractModelPrivate::connectBackend(backend);
}

void RealTimeRidesFromStationModelPrivate::disconnectBackend(AbstractBackendWrapper *backend)
{
    disconnect(backend, &AbstractBackendWrapper::realTimeRidesFromStationRegistered,
               this, &RealTimeRidesFromStationModelPrivate::slotRidesFromStationRegistered);
    AbstractModelPrivate::disconnectBackend(backend);
}

////// End of private class //////

RealTimeRidesFromStationModel::RealTimeRidesFromStationModel(QObject *parent) :
    AbstractModel(*(new RealTimeRidesFromStationModelPrivate(this)), parent)
{
}

QHash<int, QByteArray> RealTimeRidesFromStationModel::roleNames() const
{
    QHash <int, QByteArray> roles;
    roles.insert(LineRole, "line");
    roles.insert(NameRole, "name");
    return roles;
}

RealTimeStationSearchModel * RealTimeRidesFromStationModel::stationSearchModel() const
{
    Q_D(const RealTimeRidesFromStationModel);
    return d->stationSearchModel;
}

void RealTimeRidesFromStationModel::setStationSearchModel(RealTimeStationSearchModel *stationSearchModel)
{
    Q_D(RealTimeRidesFromStationModel);
    if (d->stationSearchModel != stationSearchModel) {
        if (d->stationSearchModel) {
            disconnect(d->stationSearchModel,
                       &RealTimeStationSearchModel::ridesFromStationRequested,
                       d, &RealTimeRidesFromStationModelPrivate::slotRidesFromStationRequested);
        }
        d->stationSearchModel = stationSearchModel;
        if (d->stationSearchModel) {
            connect(d->stationSearchModel, &RealTimeStationSearchModel::ridesFromStationRequested,
                    d, &RealTimeRidesFromStationModelPrivate::slotRidesFromStationRequested);
        }
        emit stationSearchModelChanged();
    }
}

}

#include "realtimeridesfromstationmodel.moc"
