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

#ifndef PT2_BACKENDDBUSWRAPPER_H
#define PT2_BACKENDDBUSWRAPPER_H

/**
 * @file dbusbackendwrapper.h
 * @short Definition of PT2::DBusBackendWrapper
 */

#include "pt2_global.h"
#include "manager/abstractbackendwrapper.h"

namespace PT2
{

class DBusBackendWrapperPrivate;

/**
 * @brief Backend wrapper that uses DBus to communicate
 *
 * This class simply implements a wrapper that uses DBus
 * to communicate between the backend and the wrapper.
 */
class PT2_EXPORT DBusBackendWrapper : public AbstractBackendWrapper
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor
     *
     * @param identifier identifier for this backend wrapper.
     * @param executable command line that launch the backend.
     * @param arguments list of arguments.
     * @param parent parent object.
     */
    explicit DBusBackendWrapper(const QString &identifier, const QString &executable,
                                const QMap<QString, QString> &arguments, QObject *parent = 0);
    /**
     * @brief Destructor
     */
    virtual ~DBusBackendWrapper();
    /**
     * @brief Request suggested stations for real time information
     * @param partialStation partial station name.
     * @return request identifier.
     */
    QString requestRealTimeSuggestedStations(const QString &partialStation);
public Q_SLOTS:
    /**
     * @brief Launch the backend
     */
    virtual void launch();
    /**
     * @brief Stop the backend
     */
    virtual void stop();
    /**
     * @brief Wait for stopped
     */
    virtual void waitForStopped();
    /**
     * @brief Kill the backend
     */
    virtual void kill();
    /**
     * @brief Register backend
     *
     * This method is used to register the backend. It should be called when the backend
     * is launched in order to know about it's capabilities.
     * Capabilities can be found in file \ref capabilitiesconstants.h
     *
     * @param capabilities backend capabilities, that are send as a list of strings.
     * @param copyright copyright.
     */
    void registerBackend(const QStringList &capabilities, const QString &copyright);
Q_SIGNALS:
    /**
     * @brief Suggested stations requested for real time information
     *
     * This is a DBus proxy signal.
     *
     * @param request request identifier.
     * @param partialStation partial station name.
     */
    void realTimeSuggestedStationsRequested(const QString &request, const QString &partialStation);

private:
    Q_DECLARE_PRIVATE(DBusBackendWrapper)
};

}

#endif // PT2_BACKENDDBUSWRAPPER_H
