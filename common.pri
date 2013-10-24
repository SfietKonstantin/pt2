isEmpty(COMMON_PRI_INCLUDED) {
COMMON_PRI_INCLUDED = 1

# name
NAME = pt2

# version
VERSION = 0.0.0

# DEPLOYMENT_PREFIX is used in debug builds for
# testing. It is defaulted as /usr, but could
# be a local path
isEmpty(DEPLOYMENT_PREFIX) {
    OVERRIDEN_DEPLOYMENT_PREFIX = /usr
} else {
    OVERRIDEN_DEPLOYMENT_PREFIX = $${DEPLOYMENT_PREFIX}
}

isEmpty(PREFIX) {
    CONFIG(release):PREFIX = /usr
    CONFIG(debug, debug|release):PREFIX = $${OVERRIDEN_DEPLOYMENT_PREFIX}
}

# Default directories
isEmpty(BINDIR) {
    BINDIR = $${PREFIX}/bin
}
isEmpty(LIBDIR) {
    LIBDIR = $${PREFIX}/lib
}

isEmpty(SHAREDIR) {
    SHAREDIR = $${PREFIX}/share/$${NAME}
}

isEmpty(OPTDIR) {
    OPTDIR = /opt/$${NAME}
}

isEmpty(INCLUDEDIR) {
    INCLUDEDIR = $${PREFIX}/include/$${NAME}
}

MODULENAME = org/SfietKonstantin/$${NAME}

# Application + data + qml folders
CONFIG(optify):{
    APPLICATION_FOLDER  = $${OPTDIR}/bin
    DATA_FOLDER = $${OPTDIR}/data
    PLUGIN_FOLDER = $${OPTDIR}/plugins
    I18N_FOLDER = $${OPTDIR}/i18n
    QML_FOLDER = $${OPTDIR}/qml
    QML_MODULE_FOLDER = $${OPTDIR}/$$MODULENAME
} else {
    APPLICATION_FOLDER  = $${BINDIR}
    DATA_FOLDER = $${SHAREDIR}/data
    PLUGIN_FOLDER = $${SHAREDIR}/plugins
    I18N_FOLDER = $${SHAREDIR}/i18n
    QML_FOLDER = $${SHAREDIR}/qml
    CONFIG(release):QML_MODULE_FOLDER = $$[QT_INSTALL_QML]/$$MODULENAME
    CONFIG(debug, debug|release):QML_MODULE_FOLDER = $${DEPLOYMENT_PREFIX}/$$[QT_INSTALL_QML]/$$MODULENAME
}

# Test folder
TEST_FOLDER = $${SHAREDIR}/tests/

# QML include folder
QML_PLUGIN_BASE_PATH = $${IMPORTDIR}/org/SfietKonstantin/
QML_PLUGIN_PATH = $${QML_PLUGIN_BASE_PATH}/$${NAME}


# Useful defines
DEFINES += 'VERSION=\'\"$${VERSION}\"\''

DEFINES += 'APPLICATION_FOLDER=\'\"$${APPLICATION_FOLDER}\"\''
DEFINES += 'PLUGIN_FOLDER=\'\"$${PLUGIN_FOLDER}\"\''
DEFINES += 'PROVIDER_PATH=\'\"$${APPLICATION_FOLDER}/$${NAME}-provider\"\''
CONFIG(debug, debug|release): 'IMPORT_FOLDER=\'\"$${DEPLOYMENT_PREFIX}/$$[QT_INSTALL_QML]\"\''

}
