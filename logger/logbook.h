#pragma once

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QDateTime>

#include "ansiescapeprivate.h"
#include "logsaver.h"


struct GlobalSettingsLog{
    QString logDir                                  { DEFAULT_LOG_DIR };
    QString prefixLogFile                           { DEFAULT_PREFIX_LOG_FILE };
    QString sufixLogFile                            { DEFAULT_SUFIX_LOG_FILE };
    int maxSizeFile                                 { 50000000 }; // 50MB
    int maxQuantityFiles                            { 5 };
    bool useAnsiEscapeCode                          { true };
};


class Logbook : public QObject
{
    Q_OBJECT

    static QList<QString> listOutput        ;
    static GlobalSettingsLog settingsLog    ;
    static QByteArray prefixDebugGraphics      ;
    static QByteArray prefixWarningGraphics    ;
    static QByteArray prefixCriticalGraphics   ;
    static QByteArray prefixFatalGraphics      ;
    static QByteArray prefixInfoGraphics       ;
public:
    explicit Logbook(const GlobalSettingsLog &globalSettingsLog, QObject *parent = nullptr);

private:
    static void myMessageOutput(const QtMsgType type, const QMessageLogContext &, const QString &msg);
    static inline const QByteArray* getPrefixGraphics(const QtMsgType type);
    static void writeToFile(const QString &msg, const QByteArray *prefixGraphics = nullptr);
    static void saveCurrentMsg(const QString &msg, const QByteArray *prefixGraphics = nullptr);
    static QString getSavingFile();
    static QStringList getListFilesInDir(const QString &dir, const bool addAbsoluteFilePath = true);
};
