#include "logbook.h"

GlobalSettingsLog Logbook::settingsLog  ;
QList<QString> Logbook::listOutput      ;
QByteArray Logbook::prefixDebugGraphics      ;
QByteArray Logbook::prefixWarningGraphics    ;
QByteArray Logbook::prefixCriticalGraphics   ;
QByteArray Logbook::prefixFatalGraphics      ;
QByteArray Logbook::prefixInfoGraphics       ;


Logbook::Logbook(const GlobalSettingsLog &globalSettingsLog, QObject *parent) :
    QObject(parent)
{
    prefixDebugGraphics      = AnsiEscapePrivate::getPrefixGraphics("#FFFFFF",  "",         EffectsLog::DEFAULT     );
    prefixWarningGraphics    = AnsiEscapePrivate::getPrefixGraphics("#FFCC00",  "",         EffectsLog::ITALICS     );
    prefixCriticalGraphics   = AnsiEscapePrivate::getPrefixGraphics("#950000",  "#FFFFFF",  EffectsLog::FADED       );
    prefixFatalGraphics      = AnsiEscapePrivate::getPrefixGraphics("#FF0000",  "#000000",  EffectsLog(EffectsLog::UNDERSCORE | EffectsLog::BOLD));
    prefixInfoGraphics       = AnsiEscapePrivate::getPrefixGraphics("#7b95de",  "",         EffectsLog::DEFAULT     );


    settingsLog = globalSettingsLog;
    settingsLog.pathFileLog = settingsLog.pathFileLog.isEmpty() ? DEFAULT_LOG_PATH : settingsLog.pathFileLog;
    QDir().mkpath(QFileInfo(settingsLog.pathFileLog).absolutePath());
    qInstallMessageHandler(myMessageOutput);
}

void Logbook::myMessageOutput(const QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    if (settingsLog.useAnsiEscapeCode && type != QtMsgType::QtDebugMsg) { // qDebug() is default
        const QByteArray *prefixGraphics = const_cast<QByteArray *>(getPrefixGraphics(type));
        fprintf(stderr, "%s%s%s\n", prefixGraphics->constData(), msg.toUtf8().constData(), AnsiEscapePrivate::postfix.constData());
        writeToFile(msg, prefixGraphics);
        return;
    }
    fprintf(stderr, "%s\n", msg.toUtf8().constData());
    writeToFile(msg);
}

const QByteArray *Logbook::getPrefixGraphics(const QtMsgType type)
{
    switch (type) {
    case QtMsgType::QtDebugMsg:
        return &prefixDebugGraphics;
    case QtMsgType::QtInfoMsg:
        return &prefixInfoGraphics;
    case QtMsgType::QtWarningMsg:
        return &prefixWarningGraphics;
    case QtMsgType::QtCriticalMsg:
        return &prefixCriticalGraphics;
    case QtMsgType::QtFatalMsg:
        return &prefixFatalGraphics;
    default:
        return &prefixDebugGraphics;
    }
}


void Logbook::writeToFile(const QString &msg, const QByteArray *prefixGraphics)
{
    QFile file(getSavingFile());
    if (!file.open(QFile::Append)) {
        saveCurrentMsg(msg, prefixGraphics); // if log file is busy
        qWarning() << "Error log append:" << file.fileName() << file.errorString() << Qt::endl;
        return;
    }

    if (!listOutput.isEmpty()) {
        for (const QString &str: listOutput) {
            file.write(str.toUtf8());
            file.write("\n");
        }
        listOutput.clear();
    }
    const static QByteArray arrPostfix(AnsiEscapePrivate::postfix + '\n');
    if (prefixGraphics) {
        file.write(prefixGraphics->constData());
        file.write(msg.toUtf8());
        file.write(arrPostfix);
    }
    else{
        file.write(msg.toUtf8());
        file.write("\n");
    }
    file.close();
}

void Logbook::saveCurrentMsg(const QString &msg, const QByteArray *prefixGraphics)
{
    if (prefixGraphics) {
        listOutput.push_back(prefixGraphics->constData());
        listOutput.push_back(msg);
        listOutput.push_back(AnsiEscapePrivate::postfix);
    }
    else{
        listOutput.push_back(msg);
    }
}

QString Logbook::getSavingFile()
{
    const QFileInfo fileInfo(settingsLog.pathFileLog);
    const QString baseNameFileLog { fileInfo.baseName() };
    QStringList listFilesLog = getListFilesInDir(fileInfo.absolutePath());
    QStringList listExistingLogs;
    for (const QString &file : listFilesLog) {
        QString baseName { QFileInfo(file).baseName() };
        QStringRef leftRef = baseName.leftRef(baseNameFileLog.size());
        if(leftRef == baseNameFileLog){
            listExistingLogs.append(file);
        }
    }
    listExistingLogs.sort();
    QString savingFile = listExistingLogs.isEmpty() ? "" : listExistingLogs.back();
    if(QFile(savingFile).size() > settingsLog.maxSizeFile || listExistingLogs.isEmpty()){
        savingFile = baseNameFileLog + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz");
        listExistingLogs.append(fileInfo.absolutePath() + "/" + savingFile + ".log");

        if (listExistingLogs.size() > settingsLog.maxQuantityFiles) {
            listExistingLogs.erase(listExistingLogs.end() - settingsLog.maxQuantityFiles, listExistingLogs.end());
            for (const QString &file : listExistingLogs) {
                QFile::remove(file);
            }
        }
    }
    return listExistingLogs.back();
}

QStringList Logbook::getListFilesInDir(const QString &dir)
{
    QStringList list;
    QDir directory(dir);

    for(const QFileInfo &info: directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {

        if(!info.isDir()){
            list.append(info.absoluteFilePath());
        }
    }
    return list;
}


