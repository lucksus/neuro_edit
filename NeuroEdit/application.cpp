#include "application.h"
#include "controller.h"
#include "guiuserinteractionadapter.h"

Application::Application():
        m_splash(QPixmap(":/images/splash")),
        m_main_window(0),
        m_timer(this)
{
    m_splash.setWindowFlags( Qt::WindowStaysOnTopHint | m_splash.windowFlags() );
}


Application& Application::instance(){
    static Application a;
    return a;
}

void Application::init(){
    m_splash.show();
    QTimer::singleShot(1500, this, SLOT(hide_splash()));

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(refresh_timeout()));
    m_timer.start(1000/30);
    Controller::instance().init();

    UserInteractionAdapter::install_instance(new GuiUserInteractionAdapter);

    m_main_window = new MainWindow(Controller::instance().simulation());
}

void Application::destroy(){
    delete m_main_window;
}

void Application::show_main_window(){
    m_main_window->show();
}

void Application::refresh_timeout(){
    emit refresh();
}

QWidget* Application::main_window(){
    return m_main_window;
}

void Application::hide_splash(){
    m_splash.finish(m_main_window);
}
