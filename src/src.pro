TEMPLATE = subdirs
SUBDIRS = lib bin
!CONFIG(semistatic): {
SUBDIRS += 3rdparty
lib.depends = 3rdparty
}
bin.depends = lib
