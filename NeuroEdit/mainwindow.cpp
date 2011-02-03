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

    QDockWidget *dock;
    //dock = new QDockWidget(tr("Neuron Potential Plot"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dock->setWidget(&m_neuron_potential_plot);
    //addDockWidget(Qt::RightDockWidgetArea,dock);

    dock = new QDockWidget(tr("Neuron properties"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_properties);
    addDockWidget(Qt::RightDockWidgetArea,dock);

    connect(&m_sim, SIGNAL(simulation_started()), this, SLOT(simulation_started()));
    connect(&m_sim, SIGNAL(simulation_stopped()), this, SLOT(simulation_stopped()));
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
    m_sim.set_network(m_network);
    m_sim.set_time_step(1);
    m_sim.set_time_ratio(10000);
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
