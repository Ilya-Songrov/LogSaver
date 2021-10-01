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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <iostream>

#include "logger/logsaver.h"

int main(int argc, char *argv[])
{

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationVersion(LogSaver::getVersionClass());

    LogSaver logSaver;
    //  LogSaver logSaver("../logtest/", "test1", ".log");

    QCommandLineParser parser;
    parser.setApplicationDescription("App description");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    qDebug() << "qDebug" << Qt::endl;
    qInfo() << "qInfo" << Qt::endl;
    qWarning() << "qWarning" << Qt::endl;
    qCritical() << "qCritical" << Qt::endl;
    //        qFatal("qFatal");

    return app.exec();
}
