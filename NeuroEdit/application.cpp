#include "application.h"
#include <QCoreApplication>

Application::Application():
        m_main_window(0),
        m_simulation(0),
        m_network(0),
        m_timer(this)
{
}


Application& Application::instance(){
    static Application a;
    return a;
}

void Application::init(){
    QCoreApplication::setOrganizationName("lucksus");
    QCoreApplication::setOrganizationDomain("lucksus.eu");
    QCoreApplication::setApplicationName("NeuroEdit");


    connect(&m_timer, SIGNAL(timeout()), this, SLOT(refresh_timeout()));
    m_timer.start(1000/30);

    m_simulation = new Simulation;
    m_network = new Network;
    m_simulation->set_network(m_network);
    m_main_window = new MainWindow(m_simulation);
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

Simulation* Application::simulation(){
    return m_simulation;
}
