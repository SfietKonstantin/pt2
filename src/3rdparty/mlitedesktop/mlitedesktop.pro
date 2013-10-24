include(../../../common.pri)
# Desktop support from mlite
# compiled as a separate lib because
# of license issues

TEMPLATE = lib
CONFIG += qt
QT = core

DEFINES += MLITE_LIBRARY

TARGET = mlitedesktop
target.path = $${LIBDIR}

HEADERS += mlite-global.h \
    mdesktopentry.h \
    mdesktopentry_p.h \
    MDesktopEntry

SOURCES += mdesktopentry.cpp

INSTALLS += target
