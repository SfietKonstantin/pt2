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

#ifndef PT2_PROVIDERPLUGINWRAPPER_H
#define PT2_PROVIDERPLUGINWRAPPER_H

/**
 * @file providerplugindbuswrapper.h
 * @short Definition of PT2::ProviderPluginDBusWrapper
 */

#include "pt2_global.h"

#include <QtCore/QObject>

namespace PT2
{

class Line;
class Station;
class Ride;
class CompanyNodeData;
class ProviderPluginDBusWrapperPrivate;

/**
 * @brief Provider plugin wrapper that uses DBus
 *
 * This class is a wrapper around the provider plugin,
 * that provides a clean method to interface with both
 * the plugin and DBus.
 *
 * It loads a plugin using load(), and performs
 * registration automatically.
 */
class PT2_EXPORT ProviderPluginDBusWrapper : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor
     * @param identifier DBus identifier.
     * @param parent parent object.
     */
    explicit ProviderPluginDBusWrapper(const QString &identifier, QObject *parent = 0);
    /**
     * @brief Destructor
     */
    virtual ~ProviderPluginDBusWrapper();
    /**
     * @brief Load a plugin
     * @param plugin the plugin to load.
     * @return if the plugin has been successfully loaded.
     */
    bool load(const QString &plugin);
protected:
    /**
     * @brief D-pointer
     */
    QScopedPointer<ProviderPluginDBusWrapperPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(ProviderPluginDBusWrapper)
};

}

#endif // PT2_PROVIDERPLUGINWRAPPER_H
