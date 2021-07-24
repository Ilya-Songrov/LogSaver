/**************************************************************************
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/.
**
**************************************************************************/

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
    settingsLog.logDir          = settingsLog.logDir.isEmpty() ? DEFAULT_LOG_DIR : QDir(settingsLog.logDir).absolutePath();
    settingsLog.prefixLogFile   = settingsLog.prefixLogFile.isEmpty() ? DEFAULT_PREFIX_LOG_FILE : settingsLog.prefixLogFile;

    QDir().mkpath(settingsLog.logDir);
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
        for (const QString &str: qAsConst(listOutput)) {
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
    const QStringList listFilesInLogDir = getListFilesInDir(settingsLog.logDir);
    QStringList listFilesWithPrefix;
    for (const QString &file : listFilesInLogDir) {
        QString baseName { QFileInfo(file).baseName() };
        QStringRef leftRef = baseName.leftRef(settingsLog.prefixLogFile.size());
        if(leftRef == settingsLog.prefixLogFile){
            listFilesWithPrefix.append(file);
        }
    }
    listFilesWithPrefix.sort();
    QString savingFile = listFilesWithPrefix.isEmpty() ? "" : listFilesWithPrefix.back();
    if(QFile(savingFile).size() > settingsLog.maxSizeFile || listFilesWithPrefix.isEmpty()){
        savingFile = settingsLog.prefixLogFile + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz");
        listFilesWithPrefix.append(settingsLog.logDir + "/" + savingFile + settingsLog.sufixLogFile);

        if (listFilesWithPrefix.size() > settingsLog.maxQuantityFiles) {
            listFilesWithPrefix.erase(listFilesWithPrefix.end() - settingsLog.maxQuantityFiles, listFilesWithPrefix.end());
            for (const QString &file : listFilesWithPrefix) {
                QFile::remove(file);
            }
        }
    }
    return listFilesWithPrefix.back();
}

QStringList Logbook::getListFilesInDir(const QString &dirName, const bool addAbsoluteFilePath)
{
    const auto entryInfoList = QDir(dirName).entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList list;
    for(const QFileInfo &info: entryInfoList) {
        if(!info.isDir()){
            list.append(addAbsoluteFilePath ? info.absoluteFilePath() : info.fileName());
        }
    }
    return list;
}
