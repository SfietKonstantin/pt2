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

#ifndef CAPABILITIESCONSTANTS_H
#define CAPABILITIESCONSTANTS_H

/**
 * @file capabilitiesconstants.h
 * @short Definition of constants that stands for backend capabilities
 */

/**
 * @short CAPABILITY_REAL_TIME_SUGGEST_STATION_FROM_STRING
 *
 * The backend is to able suggest a list of stations from a
 * provided string.
 */
#define CAPABILITY_REAL_TIME_SUGGEST_STATION_FROM_STRING "capability:real_time_suggest_station_from_string"
/**
 * @short CAPABILITY_REAL_TIME_RIDES_FROM_STATION
 *
 * The backend is able to provide a list of rides from a 
 * provided station.
 */
#define CAPABILITY_REAL_TIME_RIDES_FROM_STATION "capability:real_time_rides_from_station"
/**
 * @short CAPABILITY_REAL_TIME_SUGGEST_LINE_FROM_STRING
 *
 * The backend is able to suggest a list of lines from a 
 * provided string.
 */
#define CAPABILITY_REAL_TIME_SUGGEST_LINE_FROM_STRING "capability:real_time_suggest_line_from_string"

#endif // CAPABILITIESCONSTANTS_H
