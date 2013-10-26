include(../../../common.pri)

TARGET = ratp
TEMPLATE = lib
QT = core network sql
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib/ -l$${NAME}
!CONFIG(semistatic): {
LIBS += -L../../3rdparty/mlitedesktop/ -lmlitedesktop
}

HEADERS +=      ratp.h

SOURCES +=      ratp.cpp

OTHER_FILES +=  ratp.desktop

# deployment
target.path = $${PLUGIN_FOLDER}

desktopFile.path = $${PLUGIN_FOLDER}
desktopFile.files = $${OTHER_FILES}

db.path = $${PLUGIN_FOLDER}/ratp
db.files = ratp.db

INSTALLS += target desktopFile db
