#pragma once

#include <QDebug>
#include <QCoreApplication>
#include <QDir>

#define DEFAULT_LOG_PATH QDir::currentPath() + QString("/Log/%1.log").arg(QCoreApplication::applicationName())

class Logbook;

class LogSaver : QObject
{
    Q_OBJECT
    Q_CLASSINFO("Version", "1.0.0")
    // TODO: enums: NoAction, TimeBeforeNewLine

    Logbook *logbook;
public:
    // TODO: Change constuctor with such params: logDir and prefixLogFile
    explicit LogSaver(const QString &pathFileLog = DEFAULT_LOG_PATH, QObject *parent = nullptr);

    static QString getVersionClass();
};
