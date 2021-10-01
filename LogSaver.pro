QT -= gui

CONFIG += c++11

include(LogSaver.pri)

SOURCES += \
    main.cpp

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target




CONFIG(debug, debug | release) {
QT      += core
CONFIG += console
CONFIG -= app_bundle
}

#### build static lib ####
CONFIG(release, debug | release) {
TEMPLATE = lib
CONFIG += staticlib
TARGET  = LogSaver

DESTDIR = $$TARGET

rebuild.target = rebuild_destdir
rebuild.commands = rm -rf $$OUT_PWD/$$DESTDIR/* ; cp $$PWD/logger/logsaver.h $$OUT_PWD/$$DESTDIR
PRE_TARGETDEPS = $$rebuild.target
QMAKE_EXTRA_TARGETS += rebuild

qmakeforce.target = dummy
qmakeforce.commands = rm -f Makefile # to force rerun of qmake
qmakeforce.depends = FORCE
PRE_TARGETDEPS += $$qmakeforce.target
QMAKE_EXTRA_TARGETS += qmakeforce
}
#### build static lib ####
