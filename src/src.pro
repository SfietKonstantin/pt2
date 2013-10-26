TEMPLATE = subdirs
SUBDIRS = lib qml bin
!CONFIG(semistatic): {
SUBDIRS += 3rdparty
lib.depends = 3rdparty
}
qml.depends = lib
bin.depends = lib
