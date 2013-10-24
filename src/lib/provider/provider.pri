HEADERS += $$PWD/providerplugininterface.h \
    $$PWD/providerpluginobject.h \
    $$PWD/providerplugindbuswrapper.h

SOURCES += $$PWD/providerpluginobject.cpp \
    $$PWD/providerplugindbuswrapper.cpp

provider_headers.files = $$PWD/*.h
provider_headers.path = $${INCLUDEDIR}/provider

!CONFIG(optify): INSTALLS += provider_headers


