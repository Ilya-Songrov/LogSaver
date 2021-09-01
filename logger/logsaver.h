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

#pragma once

#include <QDebug>
#include <QCoreApplication>
#include <QDir>

#define DEFAULT_LOG_DIR             QDir::currentPath() + "/log"
#define DEFAULT_PREFIX_LOG_FILE     QCoreApplication::applicationName()
#define DEFAULT_SUFIX_LOG_FILE      ".log"

class Logbook;

class LogSaver : QObject
{
    Q_OBJECT
    Q_CLASSINFO("Version", "1.1.0")
    // TODO: enums: NoAction, TimeBeforeNewLine

    Logbook *logbook;
public:
    explicit LogSaver(const QString &logDir = DEFAULT_LOG_DIR, const QString &prefixLogFile = DEFAULT_PREFIX_LOG_FILE
            , const QString &sufixLogFile = DEFAULT_SUFIX_LOG_FILE, QObject *parent = nullptr);

    static QString getVersionClass();
};
