#include "application.h"
#include <QCoreApplication>
#include "izhikevich.h"

Application::Application():
        m_splash(QPixmap(":/images/splash")),
        m_main_window(0),
        m_simulation(0),
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

    QCoreApplication::setOrganizationName("lucksus");
    QCoreApplication::setOrganizationDomain("lucksus.eu");
    QCoreApplication::setApplicationName("neuro_edit");

    qRegisterMetaType<Point>("Point");
    qRegisterMetaType<Axon>("Axon");
    qRegisterMetaType<AxonNode>("AxonNode");
    qRegisterMetaType<DendriticNode>("DendriticNode");
    qRegisterMetaType<Izhikevich>("Izhikevich");
    qRegisterMetaType<Network>("Network");
    qRegisterMetaType<Neuron>("Neuron");
    qRegisterMetaType<Neuron*>("Neuron*");
    qRegisterMetaType<NeuronModel*>("NeuronModel*");
    qRegisterMetaType<Synapse>("Synapse");


    connect(&m_timer, SIGNAL(timeout()), this, SLOT(refresh_timeout()));
    m_timer.start(1000/30);

    m_main_window = new MainWindow(m_simulation);
}

void Application::destroy(){
    delete m_main_window;
}

void Application::show_main_window(){
    m_main_window->show();
}

void Application::close_simulation(){
    if(0 == m_simulation) return;
    m_simulation->request_stop();
    m_simulation->wait_till_finished();
    delete m_simulation;
    m_simulation = 0;
    emit new_simulation(0);
}

void Application::create_new_simulation(){
    close_simulation();
    m_simulation = new Simulation;
    m_simulation->set_network(new Network);
    emit new_simulation(m_simulation);
}

void Application::load_simulation(std::string filename){
    close_simulation();
    m_simulation = Simulation::load_from_file(filename);
    emit new_simulation(m_simulation);
}

void Application::save_simulation(std::string filename){
    m_simulation->write_to_file(filename);
}


void Application::refresh_timeout(){
    emit refresh();
}

Simulation* Application::simulation(){
    return m_simulation;
}

QWidget* Application::main_window(){
    return m_main_window;
}

void Application::hide_splash(){
    m_splash.finish(m_main_window);
}
