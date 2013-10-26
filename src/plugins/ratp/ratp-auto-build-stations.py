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

# Downloads stations from wap.ratp.fr
# run this script to update data if needed.

import urllib2
import re
import codecs
from HTMLParser import HTMLParser
import sqlite3
import unicodedata

def remove_accents(input_str):
    nkfd_form = unicodedata.normalize('NFKD', input_str)
    return u"".join([c for c in nkfd_form if not unicodedata.combining(c)])


# Constants
lineList = ["M1", "M2", "M3", "M3B", "M4", "M5", "M6", "M7", "M7B", "M8", "M9", "M10", "M11", "M12", "M13", "M14", "RA", "RB"]
replaceList = {u"Champs-Elysées Clémenceau": u"Champs-Elysées - Clémenceau", u"Charles de Gaulle Etoile": u"Charles de Gaulle - Etoile", u"Charles de Gaulle-Etoile": u"Charles de Gaulle - Etoile", u"Palais Royal Musée du Louvre": u"Palais Royal - Musée du Louvre", u"Denfert Rochereau": u"Denfert-Rochereau", u"Chatelet": u"Châtelet-Les-Halles", u"Chatelet-Les-Halles": u"Châtelet-Les-Halles"}

# Open DB
connection = sqlite3.connect('ratp.db')
c = connection.cursor()

c.execute("""CREATE TABLE IF NOT EXISTS rides 
             (id INTEGER PRIMARY KEY, lineCode STRING, lineName STRING, directionCode STRING, directionName STRING)""")
c.execute("""CREATE TABLE IF NOT EXISTS stations 
             (id INTEGER PRIMARY KEY, name STRING, nameUnaccented STRING)""")
c.execute("""CREATE TABLE IF NOT EXISTS link_station_ride 
             (rideId INTEGER, stationId INTEGER, stationCode STRING, CONSTRAINT id PRIMARY KEY (rideId, stationId))""")

rides = {}
stations = []

rideIndex = 0

def parseRide(line, rideIndex):
    print "Getting rides for line " + line
    if line[0] == "M":
        reseau = "metro"
    if line[0] == "R":
        reseau = "rer"
    
    rideIdRe = "directionsens=(.)\">"
    rideNameRE = "directionsens=.\">([^<]*)"
    
    response = urllib2.urlopen("http://wap.ratp.fr/siv/schedule?service=next&reseau=" + reseau + "&lineid=" + line + "&referer=line")
    data = response.read()
    
    rideIdList = re.findall(rideIdRe, data)
    rideNameList = re.findall(rideNameRE, data)

    for i in range(len(rideIdList)):
        direction = rideIdList[i]
        name = unicode(rideNameList[i])
        name = HTMLParser().unescape(name)
        
        if line not in rides:
            rides[line] = {}
        
        if direction not in rides[line]:
            rides[line][direction] = {"stations": [], "name": name, "index": rideIndex}
            rideIndex += 1
    return rideIndex

def parseLine(line, direction):
    print "Getting stations for line " + line + " with direction " + direction
    if line[0] == "M":
        reseau = "metro"
    if line[0] == "R":
        reseau = "rer"
    stationIdRE = "stationid=([^\"]*)"
    stationNameRE = "stationid=[^\"]*\">([^<]*)</a></div>"

    response = urllib2.urlopen("http://wap.ratp.fr/siv/schedule?service=next&reseau=" + reseau + "&lineid=" + line + "&referer=station&submitAction=Valider&stationname=*&directionsens=" + direction)
    data = response.read()
    
    stationIdList = re.findall(stationIdRE, data)
    stationNameList = re.findall(stationNameRE, data)
        
    for i in range(len(stationIdList)):
        stationName = unicode(stationNameList[i])
        stationName = HTMLParser().unescape(stationName)
        if stationName in replaceList:
           stationName = replaceList[stationName]
        if stationName not in stations:
            stations.append(stationName)
        stationIndex = stations.index(stationName)
        
        rides[line][direction]["stations"].append({"id": stationIndex, "code": stationIdList[i]})

for line in lineList:
    rideIndex = parseRide(line, rideIndex)

for line in rides:
    for direction in rides[line]:
        parseLine(line, direction)

# Insert stations
stationBatch = []
for i in range(len(stations)):
    stationBatch.append((i, stations[i], remove_accents(stations[i].lower())))
c.execute("DELETE FROM stations")
c.executemany("INSERT INTO stations VALUES (?, ?, ?)", stationBatch)
connection.commit()
    
# Insert rides
rideBatch = []
for line in rides:
    rideObject = rides[line]
    if line[0] == "M":
        name = unicode(u"Métro ")
    if line[0] == "R":
        name = unicode("RER ")
    name += unicode(line[1:])
    for direction in rides[line]:
        index = rides[line][direction]["index"]
        rideName = rides[line][direction]["name"]
        rideBatch.append((index, unicode(line), name, unicode(direction), rideName))
c.execute("DELETE FROM rides")
c.executemany("INSERT INTO rides VALUES (?, ?, ?, ?, ?)", rideBatch)
connection.commit()

# Insert link
linkBatch = []
for line in rides:
    for direction in rides[line]:
        index = rides[line][direction]["index"]
        rideName = rides[line][direction]["name"]
        for station in rides[line][direction]["stations"]:
            stationName = stations[station["id"]]
            if stationName not in rideName:
                linkBatch.append((index, station["id"], station["code"]))
        
c.execute("DELETE FROM link_station_ride")
c.executemany("INSERT INTO link_station_ride VALUES (?, ?, ?)", linkBatch)
connection.commit()

c.close()