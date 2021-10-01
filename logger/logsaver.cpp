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

#include "logsaver.h"
#include "logbook.h"

#include <QMetaClassInfo>

LogSaver::LogSaver(const QString &logDir, const QString &prefixLogFile, QObject *parent) : QObject(parent)
{
    GlobalSettingsLog globalSettingsLog;
    globalSettingsLog.logDir        = logDir;
    globalSettingsLog.prefixLogFile = prefixLogFile;
    globalSettingsLog.sufixLogFile  = ".log";
    logbook = new Logbook(globalSettingsLog, this);
    qDebug() << "\033[48;2;6;185;185m"
             << "Start" << QCoreApplication::applicationName() << QDateTime::currentDateTime().toString("yyyy:MM:dd - hh:mm:ss:zzz")
             << "applicationVersion:" << QCoreApplication::applicationVersion()
             << "\033[0m" << Qt::endl;
}

QString LogSaver::getVersionClass()
{
    return LogSaver::staticMetaObject.classInfo(LogSaver::staticMetaObject.indexOfClassInfo("Version")).value();;
}
