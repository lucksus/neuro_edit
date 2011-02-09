#include <QtGui/QApplication>
#include "application.h"

int main(int argc, char *argv[])
{
    QApplication qa(argc, argv);
    Application::instance().init();
    Application::instance().show_main_window();
    int return_value = qa.exec();
    Application::instance().destroy();
    return return_value;
}
