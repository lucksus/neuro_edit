#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"
#include "izhikevich.h"
#include <QDockWidget>
#include <QCloseEvent>
#include <boost/foreach.hpp>
#include "about_dialog.h"
#include "assert.h"

MainWindow::MainWindow(Simulation* sim, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_glscene(this),
    m_neuron_properties(this),
    m_izhikevich_system_plot_widget(this),
    m_sim(sim)
{
    ui->setupUi(this);
    m_network = m_sim->network();
    m_glscene.set_network(m_network);
    m_neuron_properties.set_network(m_network);
    setCentralWidget(&m_glscene);
    m_glscene.setFocus();
    m_sim_settings_widget.set_simulation(m_sim);

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

    dock = new QDockWidget(tr("Izhikevich system state"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_izhikevich_system_plot_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    ui->menuWindows->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Neuron properties"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_properties);
    addDockWidget(Qt::LeftDockWidgetArea,dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Neuron manipulator"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_manipulator_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    ui->menuWindows->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Simulation settings"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_sim_settings_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();




    connect(m_sim, SIGNAL(simulation_started()), this, SLOT(simulation_started()));
    connect(m_sim, SIGNAL(simulation_stopped()), this, SLOT(simulation_stopped()));
    connect(m_sim, SIGNAL(simulation_started()), &m_sim_settings_widget, SLOT(simulation_started()));
    connect(m_sim, SIGNAL(simulation_stopped()), &m_sim_settings_widget, SLOT(simulation_stopped()));
    connect(m_sim, SIGNAL(simulation_started()), &m_neuron_properties, SLOT(simulation_started()));
    connect(m_sim, SIGNAL(simulation_stopped()), &m_neuron_properties, SLOT(simulation_stopped()));

    connect(&m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), &m_neuron_properties, SLOT(show_properties_for_objects(std::set<SimulationObject*>)));
    connect(&m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), &m_neuron_manipulator_widget, SLOT(set_neurons(std::set<SimulationObject*>)));
    connect(&m_glscene, SIGNAL(neuron_selected(Neuron*)), &m_neuron_membrane_potential_widget, SLOT(set_neuron(Neuron*)));
    connect(&m_glscene, SIGNAL(neuron_selected(Neuron*)), &m_izhikevich_system_plot_widget, SLOT(set_neuron(Neuron*)));
    connect(&m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), this, SLOT(objects_selected(std::set<SimulationObject*>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSingle_Neuron_triggered(bool){
    std::set<SimulationObject*> s;
    s.insert(new Neuron(Point()));
    m_glscene.start_inserting(s);
}

void MainWindow::on_actionAxon_Node_triggered(bool){
    std::set<SimulationObject*> s;
    s.insert(new AxonNode);
    m_glscene.start_inserting(s);
}

void MainWindow::on_actionStart_Simulation_triggered(bool){
    m_sim->start();
}


void MainWindow::on_actionPause_Simulation_triggered(bool){
    m_sim->request_stop();
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
    m_sim->request_stop();
    usleep(1000);
    event->accept();
}

void MainWindow::on_actionQuit_triggered(bool){
    close();
}

void MainWindow::on_actionCut_triggered(bool b){
    on_actionCopy_triggered(b);
    BOOST_FOREACH(SimulationObject* o, m_glscene.selected_objects()){
        m_network->delete_object(o);
    }

}

void MainWindow::on_actionCopy_triggered(bool){
    std::set<SimulationObject*> objects = selected_objects_cloned_and_self_centered();
    if(objects.empty()) return;
    clear_clipboard();
    m_clipboard = objects;
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionPaste_triggered(bool){
    if(m_clipboard.empty()) return;
    ui->actionPaste->setEnabled(true);
    m_glscene.start_inserting(cloned_clipboard());
}

void MainWindow::on_actionConnect_triggered(bool){
    m_glscene.start_connecting(m_glscene.selected_objects());
}

void MainWindow::on_actionAbout_triggered(bool){
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::clear_clipboard(){
    BOOST_FOREACH(SimulationObject* o, m_clipboard){
        delete o;
    }
    m_clipboard.clear();
}

std::set<SimulationObject*> MainWindow::cloned_clipboard(){
    std::set<SimulationObject*> objects;
    BOOST_FOREACH(SimulationObject* o, m_clipboard){
        //objects.insert(o->clone());
        assert(false);
    }
    return objects;
}

std::set<SimulationObject*> MainWindow::selected_objects_cloned_and_self_centered(){
    std::set<SimulationObject*> objects;
    std::set<SpatialObject*> spatial_objects;
    BOOST_FOREACH(SimulationObject* o, m_glscene.selected_objects()){
        //SimulationObject* oc = o->clone();
        //objects.insert(oc);
        //SpatialObject* spo = dynamic_cast<SpatialObject*>(oc);
        //if(spo) spatial_objects.insert(spo);
        assert(false);
    }

    Point center;
    BOOST_FOREACH(SpatialObject* o, spatial_objects){
        center += o->position();
    }
    center /= spatial_objects.size();
    BOOST_FOREACH(SpatialObject* o, spatial_objects){
        o->set_position(o->position()-center);
    }
    return objects;
}

void MainWindow::objects_selected(std::set<SimulationObject*> objects){
    ui->actionCopy->setDisabled(objects.empty());
    ui->actionCut->setDisabled(objects.empty());
}
