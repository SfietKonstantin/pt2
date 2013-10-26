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

#include "abstractmultibackendmodel.h"
#include "abstractmultibackendmodel_p.h"
#include "manager/abstractbackendwrapper.h"
#include "manager/abstractbackendmanager.h"

namespace PT2
{

AbstractMultiBackendModelPrivate::AbstractMultiBackendModelPrivate(AbstractMultiBackendModel *q)
    : AbstractModelPrivate(q), backendManager(0)
{
}

void AbstractMultiBackendModelPrivate::slotBackendAdded(const QString &identifier,
                                                        AbstractBackendWrapper *backend)
{
    Q_UNUSED(identifier)
    connect(backend, &AbstractBackendWrapper::statusChanged,
            this, &AbstractModelPrivate::slotStatusChanged);
}


////// End of private class //////

AbstractMultiBackendModel::AbstractMultiBackendModel(AbstractMultiBackendModelPrivate &dd,
                                                     QObject *parent)
    : AbstractModel(dd, parent)
{
}

AbstractBackendManager * AbstractMultiBackendModel::backendManager() const
{
    Q_D(const AbstractMultiBackendModel);
    return d->backendManager;
}

void AbstractMultiBackendModel::setBackendManager(AbstractBackendManager *backendManager)
{
    Q_D(AbstractMultiBackendModel);
    if (d->backendManager != backendManager) {
        // Disconnect old signals
        if (d->backendManager) {
            disconnect(d->backendManager, &AbstractBackendManager::backendAdded,
                       d, &AbstractMultiBackendModelPrivate::slotBackendAdded);
            foreach (AbstractBackendWrapper *backend, d->backendManager->backends()) {
                d->disconnectBackend(backend);
            }
        }

        d->backendManager = backendManager;

        if (d->backendManager) {
            connect(d->backendManager, &AbstractBackendManager::backendAdded,
                    d, &AbstractMultiBackendModelPrivate::slotBackendAdded);
            foreach (AbstractBackendWrapper *backend, d->backendManager->backends()) {
                d->connectBackend(backend);
            }
        }

        emit backendManagerChanged();
    }
}


}
