#include <QCoreApplication>
#include <QCommandLineParser>
#include <iostream>

#include "logsaver.h"
#include "applicationnotify.h"

int main(int argc, char *argv[])
{
    try {
        ApplicationNotify<QCoreApplication> app(argc, argv);
        QCoreApplication::setApplicationVersion(LogSaver::getVersionClass());

        LogSaver logSaver;

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
    catch (ExceptionError &exceptionError) {
        std::cerr << qPrintable(exceptionError.message()) << std::endl;
    }
    catch (std::exception &exception) {
        std::cerr << exception.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception caught (...)" << std::endl;
    }
    return EXIT_FAILURE;
}
