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

#ifndef PT2_ABSTRACTMULTIBACKENDMODEL_P_H
#define PT2_ABSTRACTMULTIBACKENDMODEL_P_H

/**
 * @file abstractmultibackendmodel_p.h
 * @short Definition of PT2::AbstractMultiBackendModelPrivate
 */

#include "abstractmodel_p.h"
#include "abstractmultibackendmodel.h"

namespace PT2
{

class AbstractBackendManager;
/**
 * @internal
 * @short Private class for PT2::AbstractMultiBackendModel
 */
class AbstractMultiBackendModelPrivate: public AbstractModelPrivate
{
    Q_OBJECT
public:
    /**
     * @internal
     * @short Default constructor
     * @param q Q-pointer.
     */
    explicit AbstractMultiBackendModelPrivate(AbstractMultiBackendModel *q);
    /**
     * @internal
     * @brief Backend manager
     */
    AbstractBackendManager *backendManager;
private Q_SLOTS:
    /**
     * @internal
     * @brief Slot backend added
     * @param identifier identifier.
     * @param backend backend.
     */
    void slotBackendAdded(const QString &identifier, AbstractBackendWrapper *backend);
private:
    Q_DECLARE_PUBLIC(AbstractMultiBackendModel)
};

}


#endif // PT2_ABSTRACTMULTIBACKENDMODEL_P_H
