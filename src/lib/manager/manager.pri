HEADERS += $$PWD/abstractbackendwrapper.h \
    $$PWD/abstractbackendwrapper_p.h \
    $$PWD/dbusbackendwrapper.h \
    $$PWD/abstractbackendmanager.h \
    $$PWD/dbusbackendmanager.h \
    $$PWD/backendinfo.h \
    $$PWD/backendlistmanager.h

SOURCES += $$PWD/abstractbackendwrapper.cpp \
    $$PWD/dbusbackendwrapper.cpp \
    $$PWD/abstractbackendmanager.cpp \
    $$PWD/dbusbackendmanager.cpp \
    $$PWD/backendinfo.cpp \
    $$PWD/backendlistmanager.cpp

manager_headers.files = $$PWD/*.h
manager_headers.path = $${INCLUDEDIR}/manager

!CONFIG(optify): INSTALLS += manager_headers


