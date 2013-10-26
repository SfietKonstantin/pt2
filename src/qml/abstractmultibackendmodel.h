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

#ifndef PT2_ABSTRACTMULTIBACKENDMODEL_H
#define PT2_ABSTRACTMULTIBACKENDMODEL_H

#include "abstractmodel.h"

namespace PT2
{

class AbstractBackendManager;
class AbstractMultiBackendModelPrivate;
class AbstractMultiBackendModel : public AbstractModel
{
    Q_OBJECT
    /**
     * @short Backend manager
     */
    Q_PROPERTY(PT2::AbstractBackendManager * backendManager READ backendManager
               WRITE setBackendManager NOTIFY backendManagerChanged)
public:
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
Q_SIGNALS:
    /**
     * @brief Backend manager changed
     */
    void backendManagerChanged();
protected:
    /**
     * @brief D-pointer based constructor
     * @param dd D-pointer.
     * @param parent parent object.
     */
    explicit AbstractMultiBackendModel(AbstractMultiBackendModelPrivate &dd, QObject *parent = 0);
private:
    Q_DECLARE_PRIVATE(AbstractMultiBackendModel)
};

}

#endif // PT2_ABSTRACTMULTIBACKENDMODEL_H
