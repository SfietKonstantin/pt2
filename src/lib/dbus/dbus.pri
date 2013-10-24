HEADERS += $$PWD/dbusconstants.h \
    $$PWD/dbushelper.h

SOURCES += $$PWD/dbushelper.cpp

dbus_headers.files = $$PWD/*.h
dbus_headers.path = $${INCLUDEDIR}/dbus

!CONFIG(optify): INSTALLS += dbus_headers
