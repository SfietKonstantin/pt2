#!/bin/sh
python generate.py
rm ../src/lib/dbus/dbus-backend.xml
mv dbus-backend.xml ../src/lib/dbus/

rm ../src/lib/provider/providerplugininterface.h
rm ../src/lib/provider/providerpluginobject.h
rm ../src/lib/provider/providerpluginobject.cpp
mv providerplugininterface.h ../src/lib/provider/
mv providerpluginobject.h ../src/lib/provider/
mv providerpluginobject.cpp ../src/lib/provider/

rm ../src/lib/manager/abstractbackendwrapper.h
rm ../src/lib/manager/abstractbackendwrapper.cpp
mv abstractbackendwrapper.h ../src/lib/manager/
mv abstractbackendwrapper.cpp ../src/lib/manager/

rm ../src/lib/provider/providerplugindbuswrapper.cpp
mv providerplugindbuswrapper.cpp ../src/lib/provider/

rm ../src/lib/manager/dbusbackendwrapper.h
rm ../src/lib/manager/dbusbackendwrapper.cpp
mv dbusbackendwrapper.h ../src/lib/manager/
mv dbusbackendwrapper.cpp ../src/lib/manager/

rm ../src/lib/capabilitiesconstants.h
mv capabilitiesconstants.h ../src/lib/