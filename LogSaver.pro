QT -= gui

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Exceptions/exceptions/exceptionerror.cpp \
    Exceptions/exceptions/unixsignalnotifier.cpp \
    logger/ansiescapeprivate.cpp \
    logger/logbook.cpp \
    logger/logsaver.cpp \
    main.cpp

HEADERS += \
    Exceptions/exceptions/applicationnotify.h \
    Exceptions/exceptions/exceptionerror.h \
    Exceptions/exceptions/unixsignalnotifier.h \
    logger/ansiescapeprivate.h \
    logger/logbook.h \
    logger/logsaver.h

INCLUDEPATH += \
    Exceptions/exceptions \
    logger \

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
