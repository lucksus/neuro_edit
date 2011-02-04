#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"
#include "izhikevich.h"
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_glscene(this),
    m_neuron_properties(this)
{
    ui->setupUi(this);
    m_network = new Network;
    m_glscene.set_network(m_network);
    setCentralWidget(&m_glscene);
    m_glscene.setFocus();

    m_sim_settings_widget.set_simulation(&m_sim);
    m_sim.set_network(m_network);

    QDockWidget *dock;
    //dock = new QDockWidget(tr("Neuron Potential Plot"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dock->setWidget(&m_neuron_potential_plot);
    //addDockWidget(Qt::RightDockWidgetArea,dock);

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


    connect(&m_sim, SIGNAL(simulation_started()), this, SLOT(simulation_started()));
    connect(&m_sim, SIGNAL(simulation_stopped()), this, SLOT(simulation_stopped()));
    connect(&m_sim, SIGNAL(simulation_started()), &m_sim_settings_widget, SLOT(simulation_started()));
    connect(&m_sim, SIGNAL(simulation_stopped()), &m_sim_settings_widget, SLOT(simulation_stopped()));

    connect(&m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), &m_neuron_properties, SLOT(show_properties_for_objects(std::set<SimulationObject*>)));
    connect(&m_sim, SIGNAL(simulation_time_passed(long)), this, SLOT(simulation_time_passed(long)));
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


void MainWindow::simulation_time_passed(long){
    m_glscene.updateGL();
}
