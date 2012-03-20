#include <QtCore/QCoreApplication>
#include "application.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.processEvents();
    Application::instance().init();
    Application::instance().do_script_console();
    Application::instance().destroy();
    return Application::instance().return_value();
}
