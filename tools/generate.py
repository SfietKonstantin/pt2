#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
#
# You may use this file under the terms of the BSD license as follows:
#
# "Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#  *  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  *  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#  *  The names of its contributors may not be used to endorse or promote
#     products derived from this software without specific prior written
#     permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."

import json
from xml.dom.minidom import getDOMImplementation

defaultTypes = {"s": "QString", "i": "int "}
defaultClasses = {"s"}

def camelCase(name):
    names = name.split(" ")
    finalName = ""
    for singleName in names:
        finalName += singleName[0].upper() + singleName[1:].lower()
    return finalName[0].lower() + finalName[1:]

def getUpper(name):
    if len(name) == 0:
        return name
    return name[0].upper() + name[1:]

def makeName(data):
    return camelCase(data["class"]) + getUpper(camelCase(data["name"]))

def makeMethod(doc, type, data, objects):
    # True name
    name = makeName(data)
    
    # Generate the DBus method
    method = doc.createElement(type)
    if type == "signal":
        method.setAttribute("name", name + "Requested")
    elif type == "method":
        method.setAttribute("name", "register" + getUpper(name))
    
    # Generate the annotations
    for i in range(len(data[type])):
        parameter = data[type]["params"][i]
        if "object" in parameter:
            annotation = doc.createElement("annotation")
            annotation.setAttribute("name", "org.qtproject.QtDBus.QtTypeName.In" + str(i + 1))
            value = objects[parameter["object"]]["name"]
            if "list" in parameter and parameter["list"]:
                value = "const QList<" + value + "> &"
            annotation.setAttribute("value", value)
            method.appendChild(annotation)
        
    # Write the request (present in all methods)
    arg = doc.createElement("arg")
    arg.setAttribute("name", "request")
    arg.setAttribute("type", "s")
    if type == "method":
        arg.setAttribute("direction", "in")
    if type == "signal":
        arg.setAttribute("direction", "out")
    method.appendChild(arg)
        
    for parameter in data[type]["params"]:
        arg = doc.createElement("arg")
        arg.setAttribute("name", parameter["name"])
        paramType = ""
        if "type" in parameter:
            paramType = parameter["type"]
        elif "object" in parameter:
            paramType = objects[parameter["object"]]["type"]
        if "list" in parameter and parameter["list"]:
            paramType = "a(" + paramType + ")"
        arg.setAttribute("type", paramType)
        if type == "method":
            arg.setAttribute("direction", "in")
        if type == "signal":
            arg.setAttribute("direction", "out")
        method.appendChild(arg)
        
    return method

def makeSignature(type, data, className, prefix, suffix, addReqest = True):
    signature = ""
    if addReqest:
        signature = "void "
    else:
        signature = "QString "
    if className != "":
        signature += className + "::"
    signature += prefix
    if prefix != "":
        signature += getUpper(makeName(data))
    else:
        signature += makeName(data)
    signature += getUpper(suffix)
    signature += "("
    parametersList = []
    if addReqest:
        parametersList.append("const QString &request")
    for parameter in data[type]["params"]:
        parameterString = ""
        if "type" in parameter:
            parameterString = defaultTypes[parameter["type"]]
        if "object" in parameter:
            parameterString = objects[parameter["object"]]["name"]
        if "list" in parameter:
            parameterString = "const QList<" + parameterString + "> &"
        else:
            if ("type" in parameter and parameter["type"] in defaultClasses) \
                or ("object" in parameter):
                parameterString = "const " + parameterString + " &"
        
        parameterString += parameter["name"]
        parametersList.append(parameterString)
    signature += ", ".join(parametersList)
    signature += ")"
    return signature

def makeHeaderMethod(type, data, prefix, suffix, virtual = False, addReqest = True, doc = ""):
    header = "    /**\n"
    header += "     * @brief "
    if prefix != "":
        header += getUpper(prefix) + " " + method["name"]
    else:
        header += getUpper(method["name"])
    if suffix != "":
        header += " " + suffix
    header += " for " + method["class"] + " information\n"
    if doc != "":
        header += "     *\n"
        header += makeDoc(doc, 1)
        header += "     *\n"
    if addReqest:
        header += "     * @param request request identifier.\n"
    for parameter in method[type]["params"]:
        header += "     * @param " + parameter["name"] + " " + parameter["doc"] + ".\n"
    if not addReqest:
        header += "     * @return request identifier.\n"
    header += "     */\n"
    header += "    " 
    if virtual:
        header += "virtual "
    header += makeSignature(type, method, "", prefix, suffix, addReqest)
    if virtual:
        header += " = 0"
    header += ";\n"
    return header

def makeEnum(data):
    suffix = getUpper(camelCase(" ".join(data["capability"]["name"].split("_")).lower())) + "Type"
    return getUpper(camelCase(data["class"])) + "_" + suffix

def indent(data, indent):
    dataLines = data.split("\n")
    outputData = ""
    for dataLine in dataLines:
        outputData += " " * 4 * indent
        outputData += dataLine + "\n"
    return outputData

def makeDoc(doc, indent):
    dataLines = doc.split("\n")
    outputData = ""
    for dataLine in dataLines:
        outputData += " " * 4 * indent + " * "
        outputData += dataLine + "\n"
    return outputData

# First, we try to open JSON description file
try:
    f = open("structure.json")
except:
    print "Failed to open description file."
    exit(0)
    
data = json.load(f)

# Parse the specific objects
objects = data["objects"]

## DBUS XML ##

# Implement the base XML file
impl = getDOMImplementation()
doctype = impl.createDocumentType(
    qualifiedName = "node",
    publicId = "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN", 
    systemId = "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd"
)

doc = impl.createDocument(None, "node", doctype)
docElement = doc.documentElement
interfaceElement = doc.createElement("interface")
interfaceElement.setAttribute("name", "org.SfietKonstantin.pt2")
docElement.appendChild(interfaceElement)

# Add the default methods
registerElement = doc.createElement("method")
registerElement.setAttribute("name", "registerBackend")
registerElementArg = doc.createElement("arg")
registerElementArg.setAttribute("name", "capabilities")
registerElementArg.setAttribute("type", "as")
registerElementArg.setAttribute("direction", "in")
registerElement.appendChild(registerElementArg)
registerElementArg = doc.createElement("arg")
registerElementArg.setAttribute("name", "copyright")
registerElementArg.setAttribute("type", "s")
registerElementArg.setAttribute("direction", "in")
registerElement.appendChild(registerElementArg)
interfaceElement.appendChild(registerElement)

registerError = doc.createElement("method")
registerError.setAttribute("name", "registerError")
registerErrorArg = doc.createElement("arg")
registerErrorArg.setAttribute("name", "request")
registerErrorArg.setAttribute("type", "s")
registerErrorArg.setAttribute("direction", "in")
registerError.appendChild(registerErrorArg)
registerErrorArg = doc.createElement("arg")
registerErrorArg.setAttribute("name", "errorId")
registerErrorArg.setAttribute("type", "s")
registerErrorArg.setAttribute("direction", "in")
registerError.appendChild(registerErrorArg)
registerErrorArg = doc.createElement("arg")
registerErrorArg.setAttribute("name", "error")
registerErrorArg.setAttribute("type", "s")
registerErrorArg.setAttribute("direction", "in")
registerError.appendChild(registerErrorArg)
interfaceElement.appendChild(registerError)


for method in data["methods"]:
    interfaceElement.appendChild(makeMethod(doc, "signal", method, objects))
    interfaceElement.appendChild(makeMethod(doc, "method", method, objects))
    
f = open("dbus-backend.xml", "w")
f.write(doc.toprettyxml(indent = "    ", newl = "\n"))
f.close()

## Provider ##

copyright = """/*
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

"""
 
header = copyright
header += """

#ifndef PT2_PROVIDERPLUGININTERFACE_H
#define PT2_PROVIDERPLUGININTERFACE_H

/**
 * @file providerplugininterface.h
 * @short Definition of PT2::ProviderPluginInterface
 */

#include <QtCore/QStringList>

namespace PT2
{

"""
for object in objects:
    header += "class " + object + ";\n"

header += """/**
 * @brief Interface for a provider plugin
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
 * For more information about creating a provider plugin,
 * see \\ref PT2::ProviderPluginObject.
 *
 */
class ProviderPluginInterface
{
public:
    /**
     * @brief ~ProviderPluginInterface
     */
    virtual ~ProviderPluginInterface() {}
    /**
     * @brief Capabilities
     * @return capabilities.
     */
    virtual QStringList capabilities() const = 0;
    /**
     * @brief Copyright
     * @return copyright.
     */
    virtual QString copyright() const = 0;
"""

for method in data["methods"]:
    header += makeHeaderMethod("signal", method, "retrieve", "", True)

header += """};

}

Q_DECLARE_INTERFACE(PT2::ProviderPluginInterface,
                    "org.SfietKonstantin.pt2.Plugin.ProviderPluginInterface/1.0")

#endif // PT2_PROVIDERPLUGININTERFACE_H
"""
 
f = open("providerplugininterface.h", "w")
f.write(header)
f.close()

header = copyright
header += """

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
 * file \\ref capabilitiesconstants.h. 
 * 
 * copyright() provides the copyright of the provider and
 * must be implemented too.
 * 
 * Other methods are used to perform tasks that backends 
 * perform. These methods are
"""

for method in data["methods"]:
    header += " * - retrieve" + getUpper(makeName(method)) + "()\n"
    
header += """ *
 * These methods are already implemented by default, but do nothing.
 * In order to perform a task, a backend should implement some
 * of these methods.
 *
 * All calls are done asynchronously, and replies to called methods
 * are done though signals. The corresponding signals to the previously
 * listed methods are
"""

for method in data["methods"]:
    header += " * - " + makeName(method) + "Retrieved()\n"

header += """ *
 * A specific signal, errorRetrieved() can also be sent in order
 * to inform that there were an error. Error categories can be found
 * in file \\ref errorid.h
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
"""

for method in data["methods"]:
    header += makeHeaderMethod("signal", method, "retrieve", "")

header += """Q_SIGNALS:
    /**
     * @brief Error retrieved
     * @param request request identifier.
     * @param errorId a predefined string that provides the error category.
     * @param error a human-readable string describing the error.
     */
    void errorRetrieved(const QString &request, const QString &errorId, const QString &error);
"""

for method in data["methods"]:
    header += makeHeaderMethod("method", method, "", "retrieved")

header += """};

}

#endif // PT2_PROVIDERPLUGINOBJECT_H
"""

f = open("providerpluginobject.h", "w")
f.write(header)
f.close()

source = copyright

source += """
/**
 * @file providerpluginobject.cpp
 * @short Implementation of PT2::ProviderPluginObject
 */

#include "providerpluginobject.h"
#include "errorid.h"

namespace PT2
{

ProviderPluginObject::ProviderPluginObject(QObject *parent):
    QObject(parent)
{
}

ProviderPluginObject::~ProviderPluginObject()
{
}

"""

for method in data["methods"]:
    capability = "CAPABILITY_"
    capability += "_".join(method["class"].split(" ")).upper() + "_" + method["capability"]["name"]
    
    source += makeSignature("signal", method, "ProviderPluginObject", "retrieve", "") + "\n"
    source += "{\n"
    source += "    Q_UNUSED(request)\n"
    for parameter in method["signal"]["params"]:
        source += "    Q_UNUSED(" + parameter["name"] + ")\n"
    source += "    emit errorRetrieved(request, ERROR_NOT_IMPLEMENTED,\n"
    source += "                        tr(\"" + capability + " is not implemented\"));\n"
    source += "}\n"
                        

source += """
}
"""

f = open("providerpluginobject.cpp", "w")
f.write(source)
f.close()

## Manager ##

header = copyright

header += """#ifndef PT2_ABSTRACTBACKENDWRAPPER_H
#define PT2_ABSTRACTBACKENDWRAPPER_H

/**
 * @file abstractbackendwrapper.h
 * @short Definition of PT2::AbstractBackendWrapper
 */


#include "pt2_global.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace PT2
{

"""

for object in objects:
    header += "class " + object + ";\n"

header += """class AbstractBackendWrapperPrivate;

/**
 * @brief Base class for a backend wrapper
 *
 * This class provides a base for any backend wrapper.
 * A backend wrapper is an interface between a backend
 * that is usually a process that is spawned, and that is
 * used to perform specific tasks in an asynchronous ways.
 *
 * This class is used to perform the interface between the
 * spawned backend and the applications that is used to
 * display informations.
 *
 * In order to do that, the backend wrapper provides methods
 * like launch(), stop() and kill() that should be implemented,
 * as well as a set of properties, like status(), that is used
 * to get the current backend status.
 *
 * In order to be managed more easily, all backend wrappers have
 * an identifier, that is passed in the constructor, and that
 * should be unique for each backend. This identifier can also
 * be passed to the backend if needed.
 *
 * @section capabilities Capabilities
 *
 * A backend have capabilities that might vary from backend to
 * backend. Those capabilities provides what the backend can
 * deliver. A sane application should never call methods that
 * are related to a capability that is not provided by the backend,
 * as the behaviour might be unknown.
 *
 * @section managingBackend Managing backend
 *
 * The backend wrapper is used to manage backend and provide
 * a neat interface of the backend to the application. When
 * a backend is launched, the status should be set to
 * AbstractBackendWrapper::Launching. When it is being stopped,
 * it should be set to AbstractBackendWrapper::Stopping. While the
 * backend ping the wrapper back, it should also register its
 * capabilities. While an error occur in the backend (like failing
 * to register a DBus service, for example), the
 * AbstractBackendWrapper::Invalid status should be set.
 *
 * Three helper methods are used to help implementing backend wrapper:
 * - setStatus()
 * - setLastError()
 * - setBackendProperties()
 *
 * @section relayingReplies Relaying replies
 *
 * This class is also used to relay replies from the backend,
 * using specific combinaison of signals and slots. When the
 * backend replied, a slot like \\b registerAbc should be called.
 * This slot will emit \\b abcRegistered. Here is the list of
 * signals / slots used for the relay
 * - registerError()
"""

for method in data["methods"]:
    header += " * - register" + getUpper(makeName(method)) + "()\n"

header += """ * and
 * - errorRegistered()
"""
for method in data["methods"]:
    header += " * - " + makeName(method) + "Registered()\n"
header += """ *
 * This class also provides interfaces for implementing some capabilities
 * of the providers, that should be implemented in subclasses. They are all
 * of the form \\b requestAbc.
"""

for method in data["methods"]:
    header += " * - request" + getUpper(makeName(method)) + "()\n"

header += """ *
 * All these requests returns a request identifier, and all responses will
 * provide the same identifier, in order to identify the request more easily.
 *
 * Implementing requests can be done by calling createRequest(). This method
 * provides a request identifier, and register the request as pending. When
 * requests are answered, they are removed. The abstract backend wrapper can
 * then takes care of request tracking.
 *
 * Remark that there is no request for capabilities or copyright. It is
 * because registering capabilities and copyright is something that backends
 * should do automatically, in order to be validated. Subclasses should implement
 * a method to perform this registration (that should use setBackendProperties())
 * and that should set the status to AbstractBackendWrapper::Launched.
 *
 */
class PT2_EXPORT AbstractBackendWrapper: public QObject
{
    Q_OBJECT
    /**
     * @short Status
     */
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    /**
     * @brief Enumeration describing backend status
     */
    enum Status {
        /**
         * @short The backend is stopped
         */
        Stopped,
        /**
         * @short The backend is launching
         */
        Launching,
        /**
         * @short The backend is launched
         */
        Launched,
        /**
         * @short The backend is stopping
         */
        Stopping,
        /**
         * @short The backend is in an invalid state
         */
        Invalid
    };

    /**
     * @brief Enumeration describing request types
     */
    enum RequestType {
"""
for method in data["methods"]:
    header += "        /**\n"
    header += "         * @short Request " + method["name"] + " for " + method["class"] 
    header += " information\n"
    header += "         */\n"
    header += "        " + makeEnum(method) + ",\n"
header = header[0:-2]

header += """
    };

    /**
     * @brief Default constructor
     *
     * A set of properties can be passed to the constructor.
     * Those arguments can be used in the implementation to
     * provide, for example, additional command line arguments,
     * to the backend that controlled by this wrapper.
     *
     * @param identifier identifier for this backend wrapper.
     * @param executable command line that launch the backend.
     * @param arguments list of arguments.
     * @param parent parent object.
     */
    explicit AbstractBackendWrapper(const QString &identifier, const QString &executable,
                                    const QMap<QString, QString> &arguments, QObject *parent = 0);
    /**
     * @brief Destructor
     */
    virtual ~AbstractBackendWrapper();
    /**
     * @brief Identifier
     * @return identifier.
     */
    QString identifier() const;
    /**
     * @brief Status
     * @return status.
     */
    Status status() const;
    /**
     * @brief Last error
     * @return last error.
     */
    QString lastError() const;
    /**
     * @brief Capabilities
     * @return capabilities.
     */
    QStringList capabilities() const;
    /**
     * @brief Copyright
     * @return copyright.
     */
    QString copyright() const;
"""
for method in data["methods"]:
    header += makeHeaderMethod("signal", method, "request", "", True, False)

header += """public Q_SLOTS:
    /**
     * @brief Launch the backend
     *
     * Launch the backend. Implementation can use the
     * executable and the arguments that are passed
     * while an instance of the backend wrapper is created
     * to perform custom launches.
     */
    virtual void launch() = 0;
    /**
     * @brief Stop the backend
     *
     * Stops the backend. The stop is asynchronous, and can
     * occur a bit after.
     */
    virtual void stop() = 0;
    /**
     * @brief Wait for stopped
     *
     * This method can be reimplemented to wait for the backend
     * to finish cleanly.
     */
    virtual void waitForStopped();
    /**
     * @brief Kill the backend
     *
     * Kills the backend. The backend should be immediately
     * stopped, as this method is called either in emergency
     * situations.
     */
    virtual void kill() = 0;
    /**
     * @brief Register error
     *
     * This method is used to reply to any request and to note that there were an
     * error during the request. Error categories can be found in file @ref errorid.h
     *
     * @param request request identifier.
     * @param errorId a predefined string that provides the error category.
     * @param error a human-readable string describing the error.
     */
    void registerError(const QString &request, const QString &errorId, const QString &error);
"""

for method in data["methods"]:
    header += makeHeaderMethod("method", method, "register", "", False, True, method["doc"])

header += """Q_SIGNALS:
    /**
     * @brief Status changed
     */
    void statusChanged();
    /**
     * @brief Capabilities changed
     */
    void capabilitiesChanged();
    /**
     * @brief Copyright changed
     */
    void copyrightChanged();
    /**
     * @brief Error registered
     *
     * This signal is used to relay registered errors.
     *
     * @param request request identifier.
     * @param errorId a predefined string that provides the error category.
     * @param error a human-readable string describing the error.
     */
    void errorRegistered(const QString &request, const QString &errorId, const QString &error);
"""
for method in data["methods"]:
    doc = "This signal is used to relay registered " + method["name"] + " for " + method["class"] 
    doc += " information"
    header += makeHeaderMethod("method", method, "", "registered", False, True, doc)
header += """protected:
    /**
     * @brief D-pointer based constructor
     * @param dd d-pointer.
     * @param parent parent object.
     */
    explicit AbstractBackendWrapper(AbstractBackendWrapperPrivate &dd, QObject *parent);
    /**
     * @brief Executable
     * @return executable.
     */
    QString executable() const;
    /**
     * @brief Arguments
     * @return arguments.
     */
    QMap<QString, QString> arguments() const;
    /**
     * @brief Set status
     * @param status status to set.
     */
    void setStatus(Status status);
    /**
     * @brief Set last error
     * @param lastError error to set.
     */
    void setLastError(const QString &lastError);
    /**
     * @brief Set backend properties
     *
     * This method is used to set the properties of a
     * backend, that are the capabilities and the
     * copyright.
     *
     * @param capabilities capabilities to set.
     * @param copyright copyright to set.
     */
    void setBackendProperties(const QStringList &capabilities, const QString &copyright);
    /**
     * @brief Create request
     *
     * This method is used to create a request, and register it in the
     * backend wrapper. This method is useful whenever a request is
     * performed, in order to get a request identifier.
     *
     * @param requestType request type.
     * @return request identifier.
     */
    QString createRequest(RequestType requestType);

    /**
     * @brief D-pointer
     */
    QScopedPointer<AbstractBackendWrapperPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(AbstractBackendWrapper)
};

}

#endif // PT2_ABSTRACTBACKENDWRAPPER_H
"""

f = open("abstractbackendwrapper.h", "w")
f.write(header)
f.close()

source = copyright
source += """/**
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

"""

for method in data["methods"]:
    source += makeSignature("method", method, "AbstractBackendWrapper", "register", "") + "\n"
    source += "{\n"
    source += "    Q_D(AbstractBackendWrapper);\n"
    source += "    if (d->requests.contains(request)) {\n"
    source += "        if (d->requests.value(request)->type != AbstractBackendWrapper::" + makeEnum(method) + ") {\n"
    source += "            registerError(request, ERROR_INVALID_REQUEST_TYPE, \"Invalid request type\");\n"
    source += "            return;\n"
    source += "        }\n"
    source += "\n"
    
    if "source" in method:
        source += indent(method["source"], 2)
        source += "\n"
        
    source += "        delete d->requests.take(request);\n"
    
    argumentList = ["request"]
    for parameter in method["method"]["params"]:
        argumentList.append(parameter["name"])
    source += "        emit " + makeName(method) + "Registered(" + ", ".join(argumentList) + ");\n"
    source += "    }\n"
    source += "}\n\n"

source += """QString AbstractBackendWrapper::executable() const
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
"""

f = open("abstractbackendwrapper.cpp", "w")
f.write(source)
f.close()

## DBus stuff ##

source = copyright
source += """/**
 * @file providerplugindbuswrapper.cpp
 * @short Implementation of PT2::ProviderPluginDBusWrapper
 */


#include "providerplugindbuswrapper.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QPluginLoader>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusServiceWatcher>

#include "debug.h"
#include "dbus/dbusconstants.h"
#include "dbus/dbushelper.h"
#include "dbus/generated/backenddbusproxy.h"
#include "provider/providerpluginobject.h"

namespace PT2
{

/**
 * @internal
 * @brief Private class for PT2::ProviderPluginDBusWrapper
 */
class ProviderPluginDBusWrapperPrivate: public QObject
{
    Q_OBJECT
public:
    /**
     * @internal
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit ProviderPluginDBusWrapperPrivate(QObject *parent = 0);
    /**
     * @internal
     * @brief Proxy
     */
    OrgSfietKonstantinPt2Interface *proxy;
    /**
     * @internal
     * @brief DBus object path
     */
    QString dbusObjectPath;
    /**
     * @internal
     * @brief Provider plugin
     */
    ProviderPluginObject *provider;
public Q_SLOTS:
    /**
     * @internal
     * @brief Slot for service unregistered
     * @param service service name.
     */
    void slotServiceUnregistered(const QString &service);
};

ProviderPluginDBusWrapperPrivate::ProviderPluginDBusWrapperPrivate(QObject *parent)
    : QObject(parent)
{
}

void ProviderPluginDBusWrapperPrivate::slotServiceUnregistered(const QString &service)
{
    Q_UNUSED(service)
    QCoreApplication::quit();
}

////// End of private class //////

ProviderPluginDBusWrapper::ProviderPluginDBusWrapper(const QString &identifier, QObject *parent):
    QObject(parent), d_ptr(new ProviderPluginDBusWrapperPrivate)
{
    Q_D(ProviderPluginDBusWrapper);
    registerDBusTypes();

    d->dbusObjectPath = QString(DBUS_BACKEND_PATH_PREFIX) + identifier;
    d->provider = 0;
    d->proxy = new OrgSfietKonstantinPt2Interface(DBUS_SERVICE, d->dbusObjectPath,
                                                  QDBusConnection::sessionBus(), this);
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher(DBUS_SERVICE,
                                                           QDBusConnection::sessionBus(),
                                                           QDBusServiceWatcher::WatchForUnregistration,
                                                           this);
    connect(watcher, &QDBusServiceWatcher::serviceUnregistered,
            d, &ProviderPluginDBusWrapperPrivate::slotServiceUnregistered);
}

ProviderPluginDBusWrapper::~ProviderPluginDBusWrapper()
{
}

bool ProviderPluginDBusWrapper::load(const QString &plugin)
{
    Q_D(ProviderPluginDBusWrapper);
    QDir dir (PLUGIN_FOLDER);
    if (!dir.exists(plugin)) {
        warning("provider-wrapper") << "The plugin" << plugin.toLocal8Bit().constData()
                                    << "cannot be found";
        return false;
    }

    QString pluginPath = dir.absoluteFilePath(plugin);
    QPluginLoader pluginLoader(pluginPath);
    QObject *pluginObject = pluginLoader.instance();
    if (!pluginObject) {
        warning("provider-wrapper") << "The plugin" << plugin.toLocal8Bit().constData()
                                    << "cannot be loaded";
        warning("provider-wrapper") << pluginLoader.errorString();
        return false;
    }

    d->provider = qobject_cast<ProviderPluginObject *>(pluginObject);
    if (!d->provider) {
        warning("provider-wrapper") << "The plugin" << plugin.toLocal8Bit().constData()
                                    << "is not valid";
        return false;
    }

    // Establish some connections
    connect(d->provider, &ProviderPluginObject::errorRetrieved,
            d->proxy, &OrgSfietKonstantinPt2Interface::registerError);
"""
for method in data["methods"]:
    source += "    connect(d->proxy, &OrgSfietKonstantinPt2Interface::"
    source += makeName(method) + "Requested,\n"
    source += "            d->provider, &ProviderPluginObject::retrieve"
    source += getUpper(makeName(method)) + ");\n"
    
    source += "    connect(d->provider, &ProviderPluginObject::"
    source += makeName(method) + "Retrieved,\n"
    source += "            d->proxy, &OrgSfietKonstantinPt2Interface::register"
    source += getUpper(makeName(method)) + ");\n"
source += """
    // Register
    debug("provider-wrapper") << "Registration from backend with pid"
                              << QCoreApplication::applicationPid();
    d->proxy->registerBackend(d->provider->capabilities(), d->provider->copyright());

    return true;
}

}

#include "providerplugindbuswrapper.moc"
"""

f = open("providerplugindbuswrapper.cpp", "w")
f.write(source)
f.close()

header = copyright

header += """#ifndef PT2_BACKENDDBUSWRAPPER_H
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
"""

for method in data["methods"]:
    header += makeHeaderMethod("signal", method, "request", "", False, False)

header += """public Q_SLOTS:
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
     * Capabilities can be found in file \\ref capabilitiesconstants.h
     *
     * @param capabilities backend capabilities, that are send as a list of strings.
     * @param copyright copyright.
     */
    void registerBackend(const QStringList &capabilities, const QString &copyright);
Q_SIGNALS:
"""
for method in data["methods"]:
    doc = "This is a DBus proxy signal."
    header += makeHeaderMethod("signal", method, "", "requested", False, True, doc)
header += """
private:
    Q_DECLARE_PRIVATE(DBusBackendWrapper)
};

}

#endif // PT2_BACKENDDBUSWRAPPER_H
"""

f = open("dbusbackendwrapper.h", "w")
f.write(header)
f.close()

source = copyright
source += """/**
 * @file dbusbackendwrapper.cpp
 * @short Implementation of PT2::DBusBackendWrapper
 */

#include "dbusbackendwrapper.h"
#include "manager/abstractbackendwrapper_p.h"

#include <QtCore/QProcess>
#include <QtCore/QStringList>
#include <QtDBus/QDBusConnection>

#include "capabilitiesconstants.h"
#include "debug.h"
#include "dbus/dbushelper.h"
#include "dbus/dbusconstants.h"
#include "dbus/generated/dbusbackendwrapperadaptor.h"

namespace PT2
{

/**
 * @internal
 * @brief Private class for PT2::DBusBackendWrapper
 */
class DBusBackendWrapperPrivate: public AbstractBackendWrapperPrivate
{
    Q_OBJECT
public:
    /**
     * @internal
     * @brief Default constructor
     * @param q Q-pointer
     */
    explicit DBusBackendWrapperPrivate(DBusBackendWrapper *q);
    /**
     * @internal
     * @brief Process
     */
    QProcess *process;
    /**
     * @internal
     * @brief DBus object path
     */
    QString dbusObjectPath;
public Q_SLOTS:
    /**
     * @internal
     * @brief Slot for read standard output
     */
    void slotReadStandardOutput();
    /**
     * @internal
     * @brief Slot for read standard error
     */
    void slotReadStandardError();
    /**
     * @internal
     * @brief Slot for process error
     * @param error error.
     */
    void slotProcessError(QProcess::ProcessError error);
    /**
     * @internal
     * @brief Slot for finished
     * @param code exit code.
     */
    void slotFinished(int code);
private:
    /**
     * @internal
     * @brief Q-pointer
     */
    DBusBackendWrapper * const q_ptr;
    Q_DECLARE_PUBLIC(DBusBackendWrapper)

};

DBusBackendWrapperPrivate::DBusBackendWrapperPrivate(DBusBackendWrapper *q):
    AbstractBackendWrapperPrivate(), q_ptr(q)
{
}

void DBusBackendWrapperPrivate::slotReadStandardOutput()
{
    process->setReadChannel(QProcess::StandardOutput);
    while (!process->atEnd()) {
        debug("backend") << process->readLine().trimmed().constData();
    }
}

void DBusBackendWrapperPrivate::slotReadStandardError()
{
    process->setReadChannel(QProcess::StandardError);
    while (!process->atEnd()) {
        debug("backend") << process->readLine().trimmed().constData();
    }
}

void DBusBackendWrapperPrivate::slotProcessError(QProcess::ProcessError error)
{
    Q_Q(DBusBackendWrapper);
    debug("backend") << "Child process send the error" << error;
    debug("backend") << process->errorString();
    QString lastError = QString("Child process send the error :\\\"%1\\\"").arg(process->errorString());
    q->setLastError(lastError);
    q->setStatus(DBusBackendWrapper::Invalid);
}

void DBusBackendWrapperPrivate::slotFinished(int code)
{
    Q_Q(DBusBackendWrapper);
    debug("backend") << "Finished with code" << code;
    debug("backend") << "Unregister DBus object" << dbusObjectPath.toLocal8Bit().constData();
    QDBusConnection::sessionBus().unregisterObject(dbusObjectPath, QDBusConnection::UnregisterTree);
    q->setStatus(AbstractBackendWrapper::Stopped);
}

////// End of private class //////

DBusBackendWrapper::DBusBackendWrapper(const QString &identifier, const QString &executable,
                                       const QMap<QString, QString> &arguments, QObject *parent):
    AbstractBackendWrapper(*(new DBusBackendWrapperPrivate(this)), parent)
{
    Q_D(DBusBackendWrapper);
    registerDBusTypes();

    d->identifier = identifier;
    d->executable = executable;
    d->arguments = arguments;

    d->process = new QProcess(this);
    connect(d->process, &QProcess::readyReadStandardOutput,
            d, &DBusBackendWrapperPrivate::slotReadStandardOutput);
    connect(d->process, &QProcess::readyReadStandardError,
            d, &DBusBackendWrapperPrivate::slotReadStandardError);
    connect(d->process, SKSIGNAL(QProcess, error, QProcess::ProcessError),
            d, &DBusBackendWrapperPrivate::slotProcessError);
    connect(d->process, SKSIGNAL(QProcess, finished, int),
            d, &DBusBackendWrapperPrivate::slotFinished);
}

DBusBackendWrapper::~DBusBackendWrapper()
{
    kill();
}

void DBusBackendWrapper::launch()
{
    Q_D(DBusBackendWrapper);

    if (identifier().isEmpty()) {
        setLastError("No identifier was set");
        setStatus(Invalid);
        return;
    }

    QByteArray object = QCryptographicHash::hash(identifier().toLocal8Bit(),
                                                 QCryptographicHash::Md5);
    QString dbusIdentifier = QString::fromLocal8Bit(object.toHex());

    // Register to DBus
    d->dbusObjectPath = DBUS_BACKEND_PATH_PREFIX;
    d->dbusObjectPath.append(dbusIdentifier);

    new Pt2Adaptor(this);
    if (!QDBusConnection::sessionBus().registerObject(d->dbusObjectPath, this)) {
        setLastError(QString("Failed to register object on path %1").arg(d->dbusObjectPath));
        setStatus(Invalid);
        return;
    }

    // Launch the backend
    setStatus(Launching);
    d->process->setWorkingDirectory(APPLICATION_FOLDER);
    QString trueExecutable = executable();
    trueExecutable.replace("$PROVIDER", QString(PROVIDER_PATH) + " --plugin ");
    trueExecutable.append(QString(" --identifier %1 ").arg(dbusIdentifier));

    debug("dbus-backend-wrapper") << "starting" << trueExecutable;

    d->process->start(trueExecutable);
}

void DBusBackendWrapper::stop()
{
    Q_D(DBusBackendWrapper);
    if (d->process->state() == QProcess::NotRunning) {
        return;
    }

    debug("dbus-backend-wrapper") << "Stop backend for"
                                  << d->dbusObjectPath.toLocal8Bit().constData()
                                  << "with pid" << d->process->pid();

    setStatus(Stopping);
    d->process->terminate();
}

void DBusBackendWrapper::waitForStopped()
{
    Q_D(DBusBackendWrapper);
    d->process->waitForFinished(5000);
    return;
}

void DBusBackendWrapper::kill()
{
    Q_D(DBusBackendWrapper);
    if (d->process->state() == QProcess::NotRunning) {
        return;
    }

    debug("dbus-backend-wrapper") << "Kill backend for"
                                  << d->dbusObjectPath.toLocal8Bit().constData()
                                  << "with pid" << d->process->pid();
    d->process->kill();
    d->process->waitForFinished(-1);
    setStatus(Stopped);
}

void DBusBackendWrapper::registerBackend(const QStringList &capabilities, const QString &copyright)
{
    Q_D(DBusBackendWrapper);
    debug("dbus-backend-wrapper") << "Begin registration of backend for"
                                  << d->dbusObjectPath.toLocal8Bit().constData();

    if (status() == Stopping || status() == Stopped) {
        warning("dbus-backend-wrapper") << "Backend for"
                                        << d->dbusObjectPath.toLocal8Bit().constData()
                                        << "is registering while not yet launched";
        kill();
        setLastError("Backend is registering while not yet launched");
        setStatus(Invalid);
        return;
    }

    if (status() == Launched) {
        warning("dbus-backend-wrapper") << "Backend for"
                                        << d->dbusObjectPath.toLocal8Bit().constData()
                                        << "is registering twice";
        kill();
        setLastError("Backend is registering twice");
        setStatus(Invalid);
        return;
    }

    if (d->status == Invalid) {
        warning("dbus-backend-wrapper") << "Backend for"
                                        << d->dbusObjectPath.toLocal8Bit().constData()
                                        << "is registering more than twice";
        return;
    }

    debug("dbus-backend-wrapper") << "End registration of backend for"
                                  << d->dbusObjectPath.toLocal8Bit().constData();
    debug("dbus-backend-wrapper") << "Capabilities of backend retrieved for"
                                  << d->dbusObjectPath.toLocal8Bit().constData();
    debug("dbus-backend-wrapper") << "List of capabilities:";
    debug("dbus-backend-wrapper") << capabilities;
    debug("dbus-backend-wrapper") << "Copyright of backend retrieved for"
                                  << d->dbusObjectPath.toLocal8Bit().constData();

    setBackendProperties(capabilities, copyright);
    setStatus(Launched);
}
"""

for method in data["methods"]:
    source += makeSignature("signal", method, "DBusBackendWrapper", "request", "", False)
    source += "{\n"
    source += "    QString request = createRequest(" + makeEnum(method) + ");\n"
    argumentList = ["request"]
    for parameter in method["signal"]["params"]:
        argumentList.append(parameter["name"])
    source += "    emit " + makeName(method) + "Requested(" + ", ".join(argumentList) + ");\n"
    source += "    return request;\n"
    source += "}\n"
    
source += """
}

#include "dbusbackendwrapper.moc"
"""

f = open("dbusbackendwrapper.cpp", "w")
f.write(source)
f.close()

## Constants ##

header = copyright
header += """#ifndef CAPABILITIESCONSTANTS_H
#define CAPABILITIESCONSTANTS_H

/**
 * @file capabilitiesconstants.h
 * @short Definition of constants that stands for backend capabilities
 */

"""
for method in data["methods"]:
    prefix = "CAPABILITY_"
    name = "_".join(method["class"].split(" ")).upper() + "_" + method["capability"]["name"]
    header += "/**\n"
    header += " * @short " + prefix + name +  "\n"
    header += " *\n"
    header += makeDoc(method["capability"]["doc"], 0)
    header += " */\n"
    header += "#define " + prefix + name + " \"capability:" + name.lower() + "\"\n"


header += """
#endif // CAPABILITIESCONSTANTS_H
"""

f = open("capabilitiesconstants.h", "w")
f.write(header)
f.close()

