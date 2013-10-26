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
 * @file abstractbackendwrapper.cpp
 * @short Implementation of PT2::AbstractBackendWrapper
 */

#include "abstractbackendwrapper.h"
#include "abstractbackendwrapper_p.h"

#include <QtCore/QUuid>

#include "debug.h"
#include "errorid.h"
#include "base/company.h"
#include "base/line.h"
#include "base/ride.h"
#include "base/station.h"

namespace PT2
{

AbstractBackendWrapperPrivate::AbstractBackendWrapperPrivate()
{
     status = AbstractBackendWrapper::Stopped;
}

////// End of private class //////

AbstractBackendWrapper::AbstractBackendWrapper(const QString &identifier, const QString &executable,
                                               const QMap<QString, QString> &arguments,
                                               QObject *parent):
    QObject(parent), d_ptr(new AbstractBackendWrapperPrivate())
{
    Q_D(AbstractBackendWrapper);
    d->identifier = identifier;
    d->executable = executable;
    d->arguments = arguments;
}

AbstractBackendWrapper::AbstractBackendWrapper(AbstractBackendWrapperPrivate &dd, QObject *parent):
    QObject(parent), d_ptr(&dd)
{

}

AbstractBackendWrapper::~AbstractBackendWrapper()
{
}

QString AbstractBackendWrapper::identifier() const
{
    Q_D(const AbstractBackendWrapper);
    return d->identifier;
}

AbstractBackendWrapper::Status AbstractBackendWrapper::status() const
{
    Q_D(const AbstractBackendWrapper);
    return d->status;
}

QString AbstractBackendWrapper::lastError() const
{
    Q_D(const AbstractBackendWrapper);
    return d->lastError;
}

QStringList AbstractBackendWrapper::capabilities() const
{
    Q_D(const AbstractBackendWrapper);
    return d->capabilities;
}

QString AbstractBackendWrapper::copyright() const
{
    Q_D(const AbstractBackendWrapper);
    return d->copyright;
}

void AbstractBackendWrapper::waitForStopped()
{
}

void AbstractBackendWrapper::registerError(const QString &request, const QString &errorId,
                                           const QString &error)
{
    Q_D(AbstractBackendWrapper);
    if (d->requests.contains(request)) {
        debug("abs-backend-wrapper") << "Request" << request << "failed";
        debug("abs-backend-wrapper") << errorId;
        debug("abs-backend-wrapper") << error;

        delete d->requests.take(request);
        emit errorRegistered(request, errorId, error);
    }
}

void AbstractBackendWrapper::registerRealTimeSuggestedStations(const QString &request, const QList<PT2::Station> &suggestedStationList)
{
    Q_D(AbstractBackendWrapper);
    if (d->requests.contains(request)) {
        if (d->requests.value(request)->type != AbstractBackendWrapper::RealTime_SuggestStationFromStringType) {
            registerError(request, ERROR_INVALID_REQUEST_TYPE, "Invalid request type");
            return;
        }

        debug("abs-backend-wrapper") << "Suggested stations registered";
        debug("abs-backend-wrapper") << "Request" << request;
        debug("abs-backend-wrapper") << "list of suggested stations";
        foreach (Station station, suggestedStationList) {
            debug("abs-backend-wrapper") << station.name();
        }

        delete d->requests.take(request);
        emit realTimeSuggestedStationsRegistered(request, suggestedStationList);
    }
}

void AbstractBackendWrapper::registerRealTimeRidesFromStation(const QString &request, const QList<PT2::CompanyNodeData> &rideList)
{
    Q_D(AbstractBackendWrapper);
    if (d->requests.contains(request)) {
        if (d->requests.value(request)->type != AbstractBackendWrapper::RealTime_RidesFromStationType) {
            registerError(request, ERROR_INVALID_REQUEST_TYPE, "Invalid request type");
            return;
        }

        

        delete d->requests.take(request);
        emit realTimeRidesFromStationRegistered(request, rideList);
    }
}

void AbstractBackendWrapper::registerRealTimeSuggestedLines(const QString &request, const QList<PT2::Line> &suggestedLineList)
{
    Q_D(AbstractBackendWrapper);
    if (d->requests.contains(request)) {
        if (d->requests.value(request)->type != AbstractBackendWrapper::RealTime_SuggestLineFromStringType) {
            registerError(request, ERROR_INVALID_REQUEST_TYPE, "Invalid request type");
            return;
        }

        

        delete d->requests.take(request);
        emit realTimeSuggestedLinesRegistered(request, suggestedLineList);
    }
}

QString AbstractBackendWrapper::executable() const
{
    Q_D(const AbstractBackendWrapper);
    return d->executable;
}

QMap<QString, QString> AbstractBackendWrapper::arguments() const
{
    Q_D(const AbstractBackendWrapper);
    return d->arguments;
}

void AbstractBackendWrapper::setStatus(Status status)
{
    Q_D(AbstractBackendWrapper);
    if (d->status != status) {
        d->status = status;
        emit statusChanged();

        debug("abs-backend-wrapper") << "Status changed to" << d->status;
    }
}

void AbstractBackendWrapper::setLastError(const QString &lastError)
{
    Q_D(AbstractBackendWrapper);
    d->lastError = lastError;
    debug("abs-backend-wrapper") << "Last error:" << lastError;
}

void AbstractBackendWrapper::setBackendProperties(const QStringList &capabilities,
                                                  const QString &copyright)
{
    Q_D(AbstractBackendWrapper);
    if (d->capabilities != capabilities) {
        d->capabilities = capabilities;
        emit capabilitiesChanged();

        debug("abs-backend-wrapper") << "Capabilities changed" << capabilities;
    }

    if (d->copyright != copyright) {
        d->copyright = copyright;

        debug("abs-backend-wrapper") << "Copyright changed" << copyright;
        emit copyrightChanged();
    }
}

QString AbstractBackendWrapper::createRequest(RequestType requestType)
{
    Q_D(AbstractBackendWrapper);
    QString request = QUuid::createUuid().toString();

    debug("abs-backend-wrapper") << "Created request (request " << request
                                 << "and type" << requestType << ")";
    RequestData *requestData = new RequestData;
    requestData->request = request;
    requestData->type = requestType;
    d->requests.insert(request, requestData);

    return request;
}


}
