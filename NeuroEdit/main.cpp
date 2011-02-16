#include <QtGui/QApplication>
#include <QSplashScreen>
#include "application.h"

int main(int argc, char *argv[])
{
    QApplication qa(argc, argv);
    QPixmap pixmap(":/images/splash");
    QSplashScreen splash(pixmap);
    splash.show();
    qa.processEvents();
    Application::instance().init();
    Application::instance().show_main_window();
    splash.finish(Application::instance().main_window());
    int return_value = qa.exec();
    Application::instance().destroy();
    return return_value;
}
