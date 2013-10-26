include(../../common.pri)

TEMPLATE = lib
TARGET = $${NAME}qml
TARGET = $$qtLibraryTarget($$TARGET)

LIBS += -L../lib -lpt2
INCLUDEPATH += ../lib/

QT += gui qml dbus
CONFIG += plugin

OTHER_FILES += qmldir
import.files = qmldir

import.path = $$QML_MODULE_FOLDER
target.path = $$QML_MODULE_FOLDER

HEADERS += backendmodel.h \
    abstractmodel.h \
    abstractmodel_p.h \
    abstractmultibackendmodel.h \
    abstractmultibackendmodel_p.h \
    realtimestationsearchmodel.h

SOURCES += plugin.cpp \
    backendmodel.cpp \
    abstractmodel.cpp \
    abstractmultibackendmodel.cpp \
    realtimestationsearchmodel.cpp

INSTALLS += target import

# translations
TS_FILE = $$OUT_PWD/pt2.ts
EE_QM = $$OUT_PWD/pt2_eng_en.qm

ts.commands += lupdate $$PWD/.. -ts $$TS_FILE
ts.CONFIG += no_check_exist no_link
ts.output = $$TS_FILE
ts.input = ..

# TODO
#ts_install.files = $$TS_FILE
#ts_install.path = /usr/share/translations/source
#ts_install.CONFIG += no_check_exist

## should add -markuntranslated "-" when proper translations are in place (or for testing)
#engineering_english.commands += lrelease -idbased $$TS_FILE -qm $$EE_QM
#engineering_english.CONFIG += no_check_exist no_link
#engineering_english.depends = ts
#engineering_english.input = $$TS_FILE
#engineering_english.output = $$EE_QM

#engineering_english_install.path = /usr/share/translations
#engineering_english_install.files = $$EE_QM
#engineering_english_install.CONFIG += no_check_exist

#QMAKE_EXTRA_TARGETS += ts engineering_english
#PRE_TARGETDEPS += ts engineering_english
#INSTALLS += ts_install engineering_english_install
