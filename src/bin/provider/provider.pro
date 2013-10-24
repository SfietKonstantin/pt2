include(../../../common.pri)

TEMPLATE = app
TARGET = $${NAME}-provider

QT = core dbus
INCLUDEPATH += ../../lib/
LIBS += -L../../lib/ -l$${NAME}
!CONFIG(semistatic): {
LIBS += -L../../3rdparty/mlitedesktop -lmlitedesktop
}

HEADERS +=  \

SOURCES +=  main.cpp

# deployment
target.path = $${APPLICATION_FOLDER}
INSTALLS += target
