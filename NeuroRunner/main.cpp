#include <QtCore/QCoreApplication>
#include "application.h"
#include "controller.h"
#include <QtCore/QFile>
#include <iostream>
#include "scriptengine.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.processEvents();

    Application::instance().init();

    if(argc > 1){
        std::cout << "Interpreting command line arguments..." << std::endl;
        for(int i=1;i<argc;i++){
            std::cout << argv[i] << std::endl;
            Controller::instance().script_engine()->evaluate(argv[i]);
        }
    }else{
        Application::instance().do_script_console();
    }

    std::cout << "Exiting..." << std::endl;
    Application::instance().destroy();
    return Application::instance().return_value();
}
