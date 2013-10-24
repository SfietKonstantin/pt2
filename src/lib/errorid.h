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

#ifndef ERRORID_H
#define ERRORID_H

/**
 * @file errorid.h
 * @short Definition of constants that describes error categories
 */

/**
 * @short ERROR_INVALID_REQUEST_TYPE
 *
 * The error is sent when the backend wrapper receive
 * a response from the backend that do not correspond
 * to the type of data that were requested.
 *
 * This error should help for debugging backends, and
 * is not displayed in a GUI.
 */
#define ERROR_INVALID_REQUEST_TYPE "error:invalid_request_type"
/**
 * @short ERROR_NOT_IMPLEMENTED
 *
 * The error is sent when the capability is not implemented
 * in the backend, but still requested.
 *
 * This error should help for debugging backends, and
 * is not displayed in a GUI.
 */
#define ERROR_NOT_IMPLEMENTED "error:not_implemented"
/**
 * @short ERROR_BACKEND_WARNING
 *
 * The error is sent because something happened in the backend
 * side, because of some external reasons. Reasons can be lack
 * of internet connection, server timeout, or invalid data retrieved
 * from a server.
 *
 * This error is displayed in a GUI, in order to help the user
 * to understand why there is a failure in an operation.
 */
#define ERROR_BACKEND_WARNING "error:backend_warning"
/**
 * @short ERROR_OTHER
 *
 * Other error happened.
 *
 * This error code should not be set, unless there is a specific
 * reason. It is not displayed in a GUI.
 */
#define ERROR_OTHER "error:other"

#endif // ERRORID_H
