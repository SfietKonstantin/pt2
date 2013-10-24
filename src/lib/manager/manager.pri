HEADERS += $$PWD/abstractbackendwrapper.h \
    $$PWD/abstractbackendwrapper_p.h \
    $$PWD/dbusbackendwrapper.h

SOURCES += $$PWD/abstractbackendwrapper.cpp \
    $$PWD/dbusbackendwrapper.cpp

manager_headers.files = $$PWD/*.h
manager_headers.path = $${INCLUDEDIR}/manager

!CONFIG(optify): INSTALLS += manager_headers


