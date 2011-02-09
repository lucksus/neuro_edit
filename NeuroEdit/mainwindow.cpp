#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"
#include "izhikevich.h"
#include <QDockWidget>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_glscene(this),
    m_neuron_properties(this),
    m_refresh_timer(this)
{
    ui->setupUi(this);
    m_network = new Network;
    m_glscene.set_network(m_network);
    m_neuron_properties.set_network(m_network);
    setCentralWidget(&m_glscene);
    m_glscene.setFocus();

    m_sim_settings_widget.set_simulation(&m_sim);
    m_sim.set_network(m_network);

    QDockWidget *dock;
    //dock = new QDockWidget(tr("Neuron Potential Plot"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dock->setWidget(&m_neuron_potential_plot);
    //addDockWidget(Qt::RightDockWidgetArea,dock);

    dock = new QDockWidget(tr("Neuron membrane potential"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_membrane_potential_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    ui->menuWindows->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Neuron properties"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_properties);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    ui->menuWindows->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Simulation settings"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_sim_settings_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();




    connect(&m_sim, SIGNAL(simulation_started()), this, SLOT(simulation_started()));
    connect(&m_sim, SIGNAL(simulation_stopped()), this, SLOT(simulation_stopped()));
    connect(&m_sim, SIGNAL(simulation_started()), &m_sim_settings_widget, SLOT(simulation_started()));
    connect(&m_sim, SIGNAL(simulation_stopped()), &m_sim_settings_widget, SLOT(simulation_stopped()));
    connect(&m_sim, SIGNAL(simulation_started()), &m_neuron_properties, SLOT(simulation_started()));
    connect(&m_sim, SIGNAL(simulation_stopped()), &m_neuron_properties, SLOT(simulation_stopped()));

    connect(&m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), &m_neuron_properties, SLOT(show_properties_for_objects(std::set<SimulationObject*>)));
    connect(&m_glscene, SIGNAL(neuron_selected(Neuron*)), &m_neuron_membrane_potential_widget, SLOT(set_neuron(Neuron*)));
    //connect(&m_sim, SIGNAL(simulation_milliseconds_passed(double)), this, SLOT(simulation_time_passed(double)));
    connect(&m_sim, SIGNAL(simulation_milliseconds_passed(double)), &m_neuron_membrane_potential_widget, SLOT(milliseconds_passed(double)));


    //refresh:
    connect(&m_refresh_timer, SIGNAL(timeout()), &m_glscene, SLOT(updateGL()));
    m_refresh_timer.start(1000/30);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSingle_Neuron_triggered(bool){
    m_network->add_object(new Izhikevich(Point(),0.02, 0.2, -65, 8));
    m_glscene.updateGL();
}

void MainWindow::on_actionStart_Simulation_triggered(bool){
    m_sim.start();
}


void MainWindow::on_actionPause_Simulation_triggered(bool){
    m_sim.request_stop();
}


void MainWindow::simulation_started(){
    ui->actionStart_Simulation->setEnabled(false);
    ui->actionPause_Simulation->setEnabled(true);
}

void MainWindow::simulation_stopped(){
    ui->actionStart_Simulation->setEnabled(true);
    ui->actionPause_Simulation->setEnabled(false);
}


void MainWindow::simulation_time_passed(double){
    m_glscene.updateGL();
}

void MainWindow::closeEvent(QCloseEvent *event){
    m_sim.request_stop();
    usleep(1000);
    event->accept();
}
