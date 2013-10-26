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
 * @internal
 * @file plugin.cpp
 * @short Implementation of pt2 QML plugin
 */

#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QtQml>

#include <QGuiApplication>
#include <QTranslator>
#include <QLocale>

#include "manager/dbusbackendmanager.h"
#include "backendmodel.h"
#include "realtimestationsearchmodel.h"
#include "realtimeridesfromstationmodel.h"

// using custom translator so it gets properly removed from qApp when engine is deleted
/**
 * @internal
 * @brief Translator used in the QML plugin
 */
class AppTranslator: public QTranslator
{
    Q_OBJECT
public:
    /**
     * @internal
     * @brief Default constructor
     * @param parent parent object.
     */
    AppTranslator(QObject *parent)
        : QTranslator(parent)
    {
        qApp->installTranslator(this);
    }
    /**
     * @internal
     * @brief Destructor
     */
    virtual ~AppTranslator()
    {
        qApp->removeTranslator(this);
    }
};

/**
 * @internal
 * @brief Definition of the pt2 QML plugin
 */
class Pt2Plugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.SfietKonstantin.pt2")

public:
    /**
     * @brief Initialize engine
     * @param engine QML engine.
     * @param uri plugin URI.
     */
    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri)
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.SfietKonstantin.pt2"));

        AppTranslator *engineeringEnglish = new AppTranslator(engine);
        AppTranslator *translator = new AppTranslator(engine);
        engineeringEnglish->load("pt2_eng_en", "/usr/share/translations");
        translator->load(QLocale(), "pt2", "-", "/usr/share/translations");
    }
    /**
     * @brief Register additional QML types
     * @param uri plugin URI.
     */
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.SfietKonstantin.pt2"));
        qmlRegisterUncreatableType<PT2::AbstractBackendManager>(uri, 1, 0, "AbstractBackendManager",
                                                                "Cannot create");
        qmlRegisterType<PT2::DBusBackendManager>(uri, 1, 0, "DBusBackendManager");
        qmlRegisterType<PT2::BackendModel>(uri, 1, 0, "BackendModel");
        qmlRegisterType<PT2::RealTimeStationSearchModel>(uri, 1, 0, "RealTimeStationSearchModel");
        qmlRegisterType<PT2::RealTimeRidesFromStationModel>(uri, 1, 0,
                                                            "RealTimeRidesFromStationModel");
    }
};

#include "plugin.moc"

