#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"
#include "izhikevich.h"
#include <QDockWidget>
#include <QCloseEvent>
#include <boost/foreach.hpp>
#include "about_dialog.h"
#include "assert.h"
#include "axon.h"
#include <QFileDialog>
#include <QFile>
#include <fstream>
#include <iostream>
#include <sstream>
#include "serializationhelper.h"
#include "application.h"

MainWindow::MainWindow(Simulation* sim, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_glscene(0),
    m_network(0),
    m_neuron_properties(this),
    m_izhikevich_system_plot_widget(this),
    m_sim(sim),
    m_property_browser(this)
{
    ui->setupUi(this);
    m_neuron_properties.set_network(m_network);
    m_sim_settings_widget.set_simulation(m_sim);

    QVBoxLayout* l = new QVBoxLayout;
    l->setContentsMargins(0,0,0,0);
    ui->center_frame->setLayout(l);

    QDockWidget *dock;
    //dock = new QDockWidget(tr("Neuron Potential Plot"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dock->setWidget(&m_neuron_potential_plot);
    //addDockWidget(Qt::RightDockWidgetArea,dock);

    dock = new QDockWidget(tr("Property Browser"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_property_browser);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Neuron membrane potential"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_membrane_potential_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Izhikevich system state"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_izhikevich_system_plot_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Neuron properties"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_properties);
    addDockWidget(Qt::LeftDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Neuron manipulator"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_manipulator_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Simulation settings"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_sim_settings_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();




    connect(m_sim, SIGNAL(simulation_started()), this, SLOT(simulation_started()));
    connect(m_sim, SIGNAL(simulation_stopped()), this, SLOT(simulation_stopped()));
    connect(m_sim, SIGNAL(simulation_started()), &m_sim_settings_widget, SLOT(simulation_started()));
    connect(m_sim, SIGNAL(simulation_stopped()), &m_sim_settings_widget, SLOT(simulation_stopped()));
    connect(m_sim, SIGNAL(simulation_started()), &m_neuron_properties, SLOT(simulation_started()));
    connect(m_sim, SIGNAL(simulation_stopped()), &m_neuron_properties, SLOT(simulation_stopped()));

    connect(&Application::instance(), SIGNAL(new_network(Network*)), this, SLOT(network_changed(Network*)));
}

void MainWindow::init_glscene(){
    if(m_glscene) delete m_glscene;
    m_glscene = 0;
    if(!m_network) return;
    m_glscene = new GLScene(this);
    m_glscene->set_network(m_network);
    ui->center_frame->layout()->addWidget(m_glscene);
    m_glscene->setFocus();
    connect(m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), &m_neuron_properties, SLOT(show_properties_for_objects(std::set<SimulationObject*>)));
    connect(m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), &m_neuron_manipulator_widget, SLOT(set_dendritic_nodes(std::set<SimulationObject*>)));
    connect(m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), &m_property_browser, SLOT(objects_selected(std::set<SimulationObject*>)));
    connect(m_glscene, SIGNAL(neuron_selected(Neuron*)), &m_neuron_membrane_potential_widget, SLOT(set_neuron(Neuron*)));
    connect(m_glscene, SIGNAL(neuron_selected(Neuron*)), &m_izhikevich_system_plot_widget, SLOT(set_neuron(Neuron*)));
    connect(m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), this, SLOT(objects_selected(std::set<SimulationObject*>)));
    connect(m_glscene, SIGNAL(user_interaction()), &m_property_browser, SLOT(read_values_from_objects()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSingle_Neuron_triggered(bool){
    std::set<SimulationObject*> s;
    s.insert(new Neuron(Point()));
    m_glscene->start_inserting(s);
}

void MainWindow::on_actionAxon_Node_triggered(bool){
    //std::set<SimulationObject*> s;
    //s.insert(new AxonNode);
    //m_glscene.start_inserting(s);

    std::set<SimulationObject*> selected = m_glscene->selected_objects();
    assert(1 == selected.size());
    AxonNode* parent = dynamic_cast<AxonNode*>(*(selected.begin()));
    assert(parent);

    AxonNode* new_node = new AxonNode(parent->neuron());
    Axon* axon = new Axon(parent->neuron(), parent, new_node);
    std::set<SimulationObject*> s;
    s.insert(new_node);
    s.insert(axon);
    m_glscene->start_inserting(s);
}

void MainWindow::on_actionDendrite_Node_triggered(bool){
    std::set<SimulationObject*> selected = m_glscene->selected_objects();
    assert(1 == selected.size());
    DendriticNode* parent = dynamic_cast<DendriticNode*>(*(selected.begin()));
    assert(parent);

    DendriticNode* new_node = new DendriticNode(parent->neuron(), parent);
    std::set<SimulationObject*> s;
    s.insert(new_node);
    m_glscene->start_inserting(s);

}

void MainWindow::on_actionSynapse_triggered(bool){

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
    m_glscene->updateGL();
}

void MainWindow::closeEvent(QCloseEvent *event){
    m_sim->request_stop();
    usleep(1000);
    event->accept();
}

void MainWindow::on_actionNew_triggered(bool){
    Application::instance().create_empty_network();
}

void MainWindow::on_actionSave_triggered(bool){
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save network"), "", tr("NeuroEdit files (*.ne)"));
    if(fileName.isEmpty()) return;
    Application::instance().save_network(fileName.toStdString());
}

void MainWindow::on_actionLoad_triggered(bool){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load network"), "", tr("NeuroEdit files (*.ne)"));
    if(fileName.isEmpty()) return;
    Application::instance().load_network(fileName.toStdString());
}

void MainWindow::on_actionClose_triggered(bool){
    Application::instance().close_network();
}

void MainWindow::on_actionQuit_triggered(bool){
    close();
}

void MainWindow::on_actionCut_triggered(bool b){
    on_actionCopy_triggered(b);
    BOOST_FOREACH(SimulationObject* o, m_glscene->selected_objects()){
        m_network->delete_object(o);
    }

}

void MainWindow::on_actionCopy_triggered(bool){
    if(m_glscene->selected_objects().empty()) return;
    SerializationHelper::instance().clear_serialization_list();
    BOOST_FOREACH(SimulationObject* object, m_glscene->selected_objects()){
        SerializationHelper::instance().add_to_serialization_list(object->neuron());
    }

    std::stringstream stream;
    SerializationHelper::instance().serialize_objects(stream, m_glscene->selected_objects());
    m_clipboard = stream.str();
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionPaste_triggered(bool){
    if(m_clipboard.empty()) return;
    ui->actionPaste->setEnabled(true);
    std::stringstream stream(m_clipboard);
    m_glscene->start_inserting(SerializationHelper::instance().deserialize_objects(stream));
}

void MainWindow::on_actionConnect_triggered(bool){
    std::set<SimulationObject*> selected = m_glscene->selected_objects();
    assert(1 == selected.size());
    SpikeEmitter* emitter = dynamic_cast<SpikeEmitter*>(*(selected.begin()));
    assert(emitter);
    m_glscene->start_connecting(emitter);
}

void MainWindow::on_actionAbout_triggered(bool){
    AboutDialog dialog;
    dialog.exec();
}


std::set<SimulationObject*> MainWindow::selected_objects_cloned_and_self_centered(){
    std::set<SimulationObject*> objects;
    std::set<SpatialObject*> spatial_objects;
    BOOST_FOREACH(SimulationObject* o, m_glscene->selected_objects()){
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

    ui->actionAxon_Node->setEnabled(false);
    ui->actionDendrite_Node->setEnabled(false);
    ui->actionSynapse->setEnabled(false);
    ui->actionConnect->setEnabled(false);

    if(objects.size() != 1) return;
    SimulationObject* object = *(objects.begin());
    AxonNode* axon_node = dynamic_cast<AxonNode*>(object);
    DendriticNode* dendritic_node = dynamic_cast<DendriticNode*>(object);
    SpikeEmitter* emitter = dynamic_cast<SpikeEmitter*>(object);

    if(axon_node){
        ui->actionAxon_Node->setEnabled(true);
        ui->actionSynapse->setEnabled(true);
    }

    if(dendritic_node){
        ui->actionDendrite_Node->setEnabled(true);
    }

    if(emitter){
        ui->actionConnect->setEnabled(true);
    }
}

void MainWindow::network_changed(Network* n){
    m_network = n;
    BOOST_FOREACH(QDockWidget* dock, m_dock_widgets){
        //removeDockWidget(dock);
        dock->hide();
    }
    init_glscene();
    if(!m_glscene) return;

    BOOST_FOREACH(QDockWidget* dock, m_dock_widgets){
        //addDockWidget(Qt::RightDockWidgetArea, dock);
        if(&m_property_browser == dock->widget())
            dock->show();
    }
}
