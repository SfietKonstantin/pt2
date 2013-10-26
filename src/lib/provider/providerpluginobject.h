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



#ifndef PT2_PROVIDERPLUGINOBJECT_H
#define PT2_PROVIDERPLUGINOBJECT_H

/**
 * @file providerpluginobject.h
 * @short Definition of PT2::ProviderPluginObject
 */

#include "pt2_global.h"

#include <QtCore/QObject>

#include "providerplugininterface.h"

namespace PT2
{

/**
 * @brief Base for a provider plugin
 *
 * A provider plugin is a Qt / C++ plugin that can
 * be load using a helper program. A provider that
 * is loaded behave exactly like any other backend.
 *
 * Creating a provider plugin have advantages, that
 * are a very simple interface, but also draw-backs,
 * like the need to compiling and linking against
 * libpt2.
 *
 * This class is the recommended base for a provider
 * plugin. It provides all the interfaces declared
 * in PT2::ProviderPluginInterface, but also offers 
 * a QObject as a base.
 *
 * @section implementing Implementing a provider plugin
 *
 * capabilities() list the capabilities of the provider,
 * and must be implemented properly. Capabilities can be found in
 * file \ref capabilitiesconstants.h. 
 * 
 * copyright() provides the copyright of the provider and
 * must be implemented too.
 * 
 * Other methods are used to perform tasks that backends 
 * perform. These methods are
 * - retrieveRealTimeSuggestedStations()
 * - retrieveRealTimeRidesFromStation()
 * - retrieveRealTimeSuggestedLines()
 *
 * These methods are already implemented by default, but do nothing.
 * In order to perform a task, a backend should implement some
 * of these methods.
 *
 * All calls are done asynchronously, and replies to called methods
 * are done though signals. The corresponding signals to the previously
 * listed methods are
 * - realTimeSuggestedStationsRetrieved()
 * - realTimeRidesFromStationRetrieved()
 * - realTimeSuggestedLinesRetrieved()
 *
 * A specific signal, errorRetrieved() can also be sent in order
 * to inform that there were an error. Error categories can be found
 * in file \ref errorid.h
 */
class PT2_EXPORT ProviderPluginObject:
        public QObject, public ProviderPluginInterface
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit ProviderPluginObject(QObject *parent = 0);
    /**
     * @brief Destructor
     */
    virtual ~ProviderPluginObject();
public Q_SLOTS:
    /**
     * @brief Retrieve suggested stations for real time information
     * @param request request identifier.
     * @param partialStation partial station name.
     */
    void retrieveRealTimeSuggestedStations(const QString &request, const QString &partialStation);
    /**
     * @brief Retrieve rides from station for real time information
     * @param request request identifier.
     * @param station station.
     */
    void retrieveRealTimeRidesFromStation(const QString &request, const PT2::Station &station);
    /**
     * @brief Retrieve suggested lines for real time information
     * @param request request identifier.
     * @param partialLine partial line name.
     */
    void retrieveRealTimeSuggestedLines(const QString &request, const QString &partialLine);
Q_SIGNALS:
    /**
     * @brief Error retrieved
     * @param request request identifier.
     * @param errorId a predefined string that provides the error category.
     * @param error a human-readable string describing the error.
     */
    void errorRetrieved(const QString &request, const QString &errorId, const QString &error);
    /**
     * @brief Suggested stations retrieved for real time information
     * @param request request identifier.
     * @param suggestedStationList suggested station list.
     */
    void realTimeSuggestedStationsRetrieved(const QString &request, const QList<PT2::Station> &suggestedStationList);
    /**
     * @brief Rides from station retrieved for real time information
     * @param request request identifier.
     * @param rideList ride list.
     */
    void realTimeRidesFromStationRetrieved(const QString &request, const QList<PT2::CompanyNodeData> &rideList);
    /**
     * @brief Suggested lines retrieved for real time information
     * @param request request identifier.
     * @param suggestedLineList suggested line list.
     */
    void realTimeSuggestedLinesRetrieved(const QString &request, const QList<PT2::Line> &suggestedLineList);
};

}

#endif // PT2_PROVIDERPLUGINOBJECT_H
