#!/bin/sh
if [ ! -d $PWD/generated ]; then
mkdir $PWD/generated
fi

XMLTOCPP="qdbusxml2cpp"
if [ -n $1 ]; then
XMLTOCPP=$1"/qdbusxml2cpp"
fi

cd generated

$XMLTOCPP -a dbusbackendwrapperadaptor -i manager/dbusbackendwrapper.h -i base/station.h \
          -l PT2::DBusBackendWrapper ../dbus-backend.xml
$XMLTOCPP -p backenddbusproxy -i base/company.h -i base/line.h -i base/ride.h -i base/station.h \
          ../dbus-backend.xml
