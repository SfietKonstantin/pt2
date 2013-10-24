TEMPLATE = subdirs
SUBDIRS = lib
!CONFIG(semistatic): {
SUBDIRS += 3rdparty
lib.depends = 3rdparty
}
