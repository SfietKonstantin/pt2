include(../../common.pri)

TEMPLATE = lib
CONFIG += qt create_prl no_install_prl create_pc
QT = core dbus

DEFINES += PT2_LIBRARY

INCLUDEPATH += ../3rdparty/mlitedesktop
!CONFIG(semistatic): {
LIBS += -L../3rdparty/mlitedesktop -lmlitedesktop
} else {
HEADERS += ../3rdparty/mlitedesktop/mlite-global.h \
    ../3rdparty/mlitedesktop/mdesktopentry.h \
    ../3rdparty/mlitedesktop/mdesktopentry_p.h
SOURCES += ../3rdparty/mlitedesktop/mdesktopentry.cpp
}


TARGET = $${NAME}
target.path = $${LIBDIR}

HEADERS += pt2_global.h \
    debug.h \
    errorid.h \
    capabilitiesconstants.h

QMAKE_PKGCONFIG_NAME = lib$$TARGET
QMAKE_PKGCONFIG_DESCRIPTION = Public transportation 2 development files
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

headers.files = $$PWD/*.h
headers.path = $${INCLUDEDIR}

INSTALLS += target
!CONFIG(optify): INSTALLS += headers

include(base/base.pri)
include(dbus/dbus.pri)
include(manager/manager.pri)
include(provider/provider.pri)
