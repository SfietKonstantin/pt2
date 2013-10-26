include(../../../common.pri)

TARGET = test
TEMPLATE = lib
QT = core
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib/ -l$${NAME}
!CONFIG(semistatic): {
LIBS += -L../../3rdparty/mlitedesktop/ -lmlitedesktop
}

HEADERS +=      test.h

SOURCES +=      test.cpp

OTHER_FILES +=  test.desktop

# deployment
target.path = $${PLUGIN_FOLDER}

desktopFile.path = $${PLUGIN_FOLDER}
desktopFile.files = $${OTHER_FILES}

INSTALLS += target desktopFile
