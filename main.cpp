//#include <QCoreApplication>
//#include <QCommandLineParser>
//#include <iostream>


//#include "logsaver.h"
//#include "build_version.h"
//#include "ApplicationNotify.h"

//int main(int argc, char *argv[])
//{
//    try {
//        ApplicationNotify<QApplication> app(argc, argv);
//        QCoreApplication::setApplicationVersion(logsaver::getVersionClass());

//        QCommandLineParser parser;
//        parser.setApplicationDescription("App description");
//        parser.addHelpOption();
//        parser.addVersionOption();
//        parser.process(app);


//        ApplicationLogLib applicationLogLib;

//        QCommandLineParser parser;
//        parser.setApplicationDescription("App description");
//        parser.addHelpOption();
//        parser.addVersionOption();
//        parser.process(a);

//        return app.exec();
//    }
//    catch (ExceptionError &exceptionError) {
//        std::cerr << qPrintable(exceptionError.message()) << std::endl;
//    }
//    catch (std::exception &exception) {
//        std::cerr << exception.what() << std::endl;
//    }
//    catch (...) {
//        std::cerr << "Unknown exception caught (...)" << std::endl;
//    }
//    return EXIT_FAILURE;
//}
