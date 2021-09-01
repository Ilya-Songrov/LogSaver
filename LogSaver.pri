SOURCES += \
    $$PWD/Exceptions/exceptions/exceptionerror.cpp \
    $$PWD/Exceptions/exceptions/unixsignalnotifier.cpp \
    $$PWD/logger/ansiescapeprivate.cpp \
    $$PWD/logger/logbook.cpp \
    $$PWD/logger/logsaver.cpp

HEADERS += \
    $$PWD/Exceptions/exceptions/applicationnotify.h \
    $$PWD/Exceptions/exceptions/exceptionerror.h \
    $$PWD/Exceptions/exceptions/unixsignalnotifier.h \
    $$PWD/logger/ansiescapeprivate.h \
    $$PWD/logger/logbook.h \
    $$PWD/logger/logsaver.h

INCLUDEPATH += \
    $$PWD/Exceptions/exceptions \
    $$PWD/logger
