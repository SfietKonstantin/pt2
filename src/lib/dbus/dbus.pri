system($$PWD/run-dbusxml2cpp.sh $$[QT_INSTALL_BINS])

HEADERS += $$PWD/dbusconstants.h \
    $$PWD/dbushelper.h \
    $$PWD/generated/dbusbackendwrapperadaptor.h \
    $$PWD/generated/backenddbusproxy.h

SOURCES += $$PWD/dbushelper.cpp \
    $$PWD/generated/dbusbackendwrapperadaptor.cpp \
    $$PWD/generated/backenddbusproxy.cpp

OTHER_FILES += dbus-backend.xml

dbus_headers.files = $$PWD/*.h
dbus_headers.path = $${INCLUDEDIR}/dbus

!CONFIG(optify): INSTALLS += dbus_headers
