HEADERS += $$PWD/transportationobject.h \
    $$PWD/company.h \
    $$PWD/line.h \
    $$PWD/ride.h \
    $$PWD/station.h \
    $$PWD/companynodedata.h \
    $$PWD/linenodedata.h \
    $$PWD/ridenodedata.h

SOURCES += $$PWD/transportationobject.cpp \
    $$PWD/company.cpp \
    $$PWD/line.cpp \
    $$PWD/ride.cpp \
    $$PWD/station.cpp \
    $$PWD/companynodedata.cpp \
    $$PWD/linenodedata.cpp \
    $$PWD/ridenodedata.cpp

base_headers.files = $$PWD/*.h
base_headers.path = $${INCLUDEDIR}/base

!CONFIG(optify): INSTALLS += base_headers
