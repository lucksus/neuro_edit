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
#include "controller.h"
#include <stdexcept>
#include <QSettings>
#include "samples.h"
#include "lineardiscriminator.h"
#include "group.h"
#include <QMessageBox>

MainWindow::MainWindow(Simulation* sim, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_glscene(0),
    m_network(0),
    m_izhikevich_system_plot_widget(this),
    m_sim(sim),
    m_property_browser(this),
    m_log_window(this),
    m_settings_dialog(this)
{
    ui->setupUi(this);

    QVBoxLayout* l = new QVBoxLayout;
    l->setContentsMargins(0,0,0,0);
    ui->center_frame->setLayout(l);

    QDockWidget *dock;
    //dock = new QDockWidget(tr("Neuron Potential Plot"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dock->setWidget(&m_neuron_potential_plot);
    //addDockWidget(Qt::RightDockWidgetArea,dock);

    dock = new QDockWidget(tr("Property Browser"), this);
    dock->setObjectName("Property Browser");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_property_browser);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Neuron membrane potential"), this);
    dock->setObjectName("Neuron membrane potential");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_neuron_membrane_potential_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Izhikevich system state"), this);
    dock->setObjectName("Izhikevich system state");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_izhikevich_system_plot_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Simulation settings"), this);
    dock->setObjectName("Simulation settings");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_sim_settings_widget);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    dock = new QDockWidget(tr("Scripts Window"), this);
    dock->setObjectName("Scripts Window");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(&m_scripts_window);
    addDockWidget(Qt::RightDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();
    dock->setFloating(true);

    dock = new QDockWidget(tr("Log Window"), this);
    dock->setObjectName("Log Window");
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setWidget(&m_log_window);
    addDockWidget(Qt::BottomDockWidgetArea,dock);
    m_dock_widgets.insert(dock);
    ui->menuWindows->addAction(dock->toggleViewAction());
    dock->hide();

    connect(&Controller::instance(), SIGNAL(new_simulation(Simulation*)), this, SLOT(simulation_changed(Simulation*)));

    populateRecentlyUsedMenu();
}

void MainWindow::init_glscene(){
    if(m_glscene) delete m_glscene;
    m_glscene = 0;
    if(!m_network) return;
    m_glscene = new GLScene(this);
    m_glscene->set_network(m_network);
    ui->center_frame->layout()->addWidget(m_glscene);
    m_glscene->setFocus();
    connect(m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), &m_property_browser, SLOT(objects_selected(std::set<SimulationObject*>)));
    connect(m_glscene, SIGNAL(neuron_selected(Neuron*)), &m_neuron_membrane_potential_widget, SLOT(set_neuron(Neuron*)));
    connect(m_glscene, SIGNAL(neuron_selected(Neuron*)), &m_izhikevich_system_plot_widget, SLOT(set_neuron(Neuron*)));
    connect(m_glscene, SIGNAL(selection_changed(std::set<SimulationObject*>)), this, SLOT(objects_selected(std::set<SimulationObject*>)));
    connect(m_glscene, SIGNAL(user_interaction()), &m_property_browser, SLOT(read_values_from_objects()));
    connect(m_glscene, SIGNAL(right_click_on_object_during_multiselection(QPoint)), this, SLOT(right_click_on_object_during_multiselection(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSingle_Neuron_triggered(bool){
    std::set<SimulationObject*> s;
    s.insert(new Neuron(Controller::instance().simulation(),Point()));
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

void MainWindow::on_actionCurrent_Inducer_triggered(bool){
    m_glscene->start_inserting_current_inducer();
}

void MainWindow::on_actionSamples_triggered(bool){
    std::set<SimulationObject*> s;
    s.insert(new Samples(Controller::instance().simulation()));
    m_glscene->start_inserting(s);
}

void MainWindow::on_actionLinear_Discriminator_triggered(bool){
    std::set<SimulationObject*> s;
    s.insert(new LinearDiscriminator(Controller::instance().simulation()));
    m_glscene->start_inserting(s);
}

void MainWindow::on_actionGroup_triggered(bool){
    std::set<SimulationObject*> s;
    s.insert(new Group(Controller::instance().simulation()));
    m_glscene->start_inserting(s);
}


void MainWindow::on_actionStart_Simulation_triggered(bool){
    if(Controller::instance().simulation()->filename() == ""){
        if(QMessageBox::No == QMessageBox::question(this, "Saving Simulation", "The Simulation must be saved before running.\nDo you want to save and proceed?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            return;
        on_actionSave_as_triggered(false);
        if(Controller::instance().simulation()->filename() == "")
            return;
    }

    Controller::instance().simulation()->start();
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
    if(m_sim){
        m_sim->request_stop();
		m_sim->wait_till_finished();
    }

    QSettings settings;
    settings.beginGroup("application");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("application");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::set_insert_actions_enabled(bool enabled){
    ui->actionSingle_Neuron->setEnabled(enabled);
    ui->actionCurrent_Inducer->setEnabled(enabled);
    ui->actionSamples->setEnabled(enabled);
    ui->actionLinear_Discriminator->setEnabled(enabled);
    ui->actionGroup->setEnabled(enabled);
}

void MainWindow::on_actionNew_triggered(bool){
    Controller::instance().create_new_simulation();
}

void MainWindow::on_actionSave_triggered(bool b){
    try{
        Controller::instance().save_simulation();
    }catch(std::logic_error){
        on_actionSave_as_triggered(b);
    }
}

void MainWindow::on_actionSave_as_triggered(bool){
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save network"), "", tr("Binary NeuroEdit files (*.neb)"));
    if(fileName.isEmpty()) return;
    if(!fileName.endsWith(".neb")) fileName.append(".neb");
    Controller::instance().save_simulation(fileName.toStdString());
    addFileToRecentlyUsed(fileName);
}

void MainWindow::on_actionExport_Simulation_to_XML_triggered(bool){
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save network"), "", tr("XML NeuroEdit files (*nex)"));
    if(fileName.isEmpty()) return;
    if(!fileName.endsWith(".nex")) fileName.append(".nex");
    Controller::instance().export_xml_simulation(fileName.toStdString());
    addFileToRecentlyUsed(fileName);
}

void MainWindow::on_actionLoad_triggered(bool){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load network"), "", tr("Binary NeuroEdit files (*.neb);;JavaScript files (*.js)"));
    if(fileName.isEmpty()) return;
    if(fileName.endsWith(".neb"))
        Controller::instance().load_simulation(fileName.toStdString());
    if(fileName.endsWith(".js"))
        m_scripts_window.load_script(fileName);
    addFileToRecentlyUsed(fileName);
}

void MainWindow::on_actionImport_Simulation_from_XML_triggered(bool){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load network"), "", tr("XML NeuroEdit files (*nex)"));
    if(fileName.isEmpty()) return;
    Controller::instance().import_xml_simulation(fileName.toStdString());
    addFileToRecentlyUsed(fileName);
}

void MainWindow::on_actionClose_triggered(bool){
    QSettings settings;
    settings.beginGroup("application");
    settings.setValue("windowState", saveState());
    Controller::instance().close_simulation();
    //set_insert_actions_enabled(false);
}

void MainWindow::on_actionSettings_triggered(bool){
    m_settings_dialog.show();
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

void MainWindow::on_actionRemove_triggered(bool){
    std::set<SimulationObject*> selected = m_glscene->selected_objects();
    m_network->delete_objects(selected);
}

void MainWindow::on_actionConnect_triggered(bool){
    std::set<SimulationObject*> selected = m_glscene->selected_objects();
    assert(1 == selected.size());
    SpikingObject* spiking_object = dynamic_cast<SpikingObject*>(*(selected.begin()));
    Samples* samples = dynamic_cast<Samples*>(*(selected.begin()));
    LinearDiscriminator* ld = dynamic_cast<LinearDiscriminator*>(*(selected.begin()));
    assert(spiking_object || samples || ld);
    m_glscene->start_connecting(*(selected.begin()));
}

void MainWindow::on_actionAbout_triggered(bool){
    AboutDialog dialog;
    dialog.exec();
}


std::set<SimulationObject*> MainWindow::selected_objects_cloned_and_self_centered(){
    std::set<SimulationObject*> objects;
    BOOST_FOREACH(SimulationObject* o, m_glscene->selected_objects()){
        //SimulationObject* oc = o->clone();
        //objects.insert(oc);
        //SpatialObject* spo = dynamic_cast<SpatialObject*>(oc);
        //if(spo) spatial_objects.insert(spo);
        assert(false);
    }

    Point center;
    BOOST_FOREACH(SimulationObject* o, objects){
        center += o->position();
    }
    center /= objects.size();
    BOOST_FOREACH(SimulationObject* o, objects){
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

    ui->menuSelection->setEnabled(objects.size()>0);

    if(objects.size() != 1) return;
    SimulationObject* object = *(objects.begin());
    AxonNode* axon_node = dynamic_cast<AxonNode*>(object);
    DendriticNode* dendritic_node = dynamic_cast<DendriticNode*>(object);
    SpikingObject* emitter = dynamic_cast<SpikingObject*>(object);
    Samples* samples = dynamic_cast<Samples*>(object);
    LinearDiscriminator* linear_discriminator = dynamic_cast<LinearDiscriminator*>(object);

    if(axon_node){
        ui->actionAxon_Node->setEnabled(true);
        ui->actionSynapse->setEnabled(true);
    }

    if(dendritic_node){
        ui->actionDendrite_Node->setEnabled(true);
    }

    if(emitter || samples || linear_discriminator){
        ui->actionConnect->setEnabled(true);
    }
}

void MainWindow::right_click_on_object_during_multiselection(QPoint pos){
    std::set<SimulationObject*> selected = m_glscene->selected_objects();
    if(selected.size() > 0){
        ui->menuSelection->popup(m_glscene->mapToGlobal(pos));
    }
}

void MainWindow::simulation_changed(Simulation* s){
    //disconnect(m_sim, SIGNAL(simulation_started()), this, SLOT(simulation_started()));
    //disconnect(m_sim, SIGNAL(simulation_stopped()), this, SLOT(simulation_stopped()));
    //disconnect(m_sim, SIGNAL(simulation_started()), &m_sim_settings_widget, SLOT(simulation_started()));
    //disconnect(m_sim, SIGNAL(simulation_stopped()), &m_sim_settings_widget, SLOT(simulation_stopped()));
    m_sim = s;
    set_insert_actions_enabled(s);
    connect(m_sim, SIGNAL(simulation_started()), this, SLOT(simulation_started()));
    connect(m_sim, SIGNAL(simulation_stopped()), this, SLOT(simulation_stopped()));
    connect(m_sim, SIGNAL(simulation_started()), &m_sim_settings_widget, SLOT(simulation_started()));
    connect(m_sim, SIGNAL(simulation_stopped()), &m_sim_settings_widget, SLOT(simulation_stopped()));
    m_sim_settings_widget.set_simulation(m_sim);

    if(m_sim) m_network = m_sim->network();
    else m_network = 0;

    BOOST_FOREACH(QDockWidget* dock, m_dock_widgets){
        //removeDockWidget(dock);
        dock->hide();
    }
    init_glscene();
    if(!m_glscene) return;

    QSettings settings;
    settings.beginGroup("application");
    QByteArray arr = settings.value("windowState").toByteArray();
    if(arr.size() == 0)
        BOOST_FOREACH(QDockWidget* dock, m_dock_widgets){
            //addDockWidget(Qt::RightDockWidgetArea, dock);
            if(&m_property_browser == dock->widget())
                dock->show();
            if(&m_log_window == dock->widget()){
                dock->show();
                dock->resize(dock->width(),50);
                dock->adjustSize();
            }
        }
    else
        restoreState(arr);
}

QStringList MainWindow::recentlyUsedFiles() const{
        QSettings settings;
        settings.beginGroup("application/recently_used");
        QStringList recentFiles;
        QString key;
        foreach(key, settings.allKeys())
                recentFiles.insert(key.toInt(), settings.value(key).toString());
        return recentFiles;
}

void MainWindow::setRecentlyUsedFiles(QStringList files){
        QSettings settings;
        settings.beginGroup("application/recently_used");
        QString key;
        foreach(key, settings.allKeys())
                settings.remove(key);
        for(int x = 0; x < files.size(); x++)
                settings.setValue(QString("%1").arg(x), files[x]);
}

void MainWindow::openRecentlyUsedFile(){
        QAction* recentFile = qobject_cast<QAction*>(sender());
        QString filename = recentFile->text();
        if(filename.endsWith(".neb"))
            if(recentFile) Controller::instance().load_simulation(filename.toStdString());
        if(filename.endsWith(".js"))
            m_scripts_window.load_script(filename);
}

void MainWindow::populateRecentlyUsedMenu(){
        ui->menuOpen_Recent->clear();
        QString filePath;
        QAction* action;
        foreach(filePath, recentlyUsedFiles()){
                if(!filePath.isEmpty()){
                        action = ui->menuOpen_Recent->addAction(filePath);
                        connect(action, SIGNAL(triggered()), this, SLOT(openRecentlyUsedFile()));
                }
        }
        if(ui->menuOpen_Recent->isEmpty()) return;
        ui->menuOpen_Recent->addSeparator();
        action = ui->menuOpen_Recent->addAction("Clear");
        connect(action, SIGNAL(triggered()), this, SLOT(clearRecentlyUsedFiles()));
}

void MainWindow::addFileToRecentlyUsed(QString filePath){
        QList<QString> recentFiles = recentlyUsedFiles();
        if(!recentFiles.empty())
                if(recentFiles[0] == filePath) return;
        recentFiles.removeAll(filePath);
        recentFiles.prepend(filePath);
        setRecentlyUsedFiles(recentFiles);
        populateRecentlyUsedMenu();
}

void MainWindow::clearRecentlyUsedFiles(){
        setRecentlyUsedFiles(QList<QString>());
        populateRecentlyUsedMenu();
}

void MainWindow::populateAddToGroupMenu(){
    ui->menuAdd_to_Group->clear();
    if(!m_network) return;
    QAction* action;
    BOOST_FOREACH(SimulationObject* o, m_network->objects_as_std_set()){
        Group* group = dynamic_cast<Group*>(o);
        if(group){
            action = ui->menuAdd_to_Group->addAction(group->objectName());
            connect(action, SIGNAL(triggered()), this, SLOT(add_selected_to_group()));
        }
    }
}

void MainWindow::add_selected_to_group(){
    QAction* group_action = qobject_cast<QAction*>(sender());
    if(group_action){
        BOOST_FOREACH(SimulationObject* o, m_network->objects_as_std_set()){
            Group* group = dynamic_cast<Group*>(o);
            if(group && group->objectName() == group_action->text()){
                BOOST_FOREACH(SimulationObject* o, m_glscene->selected_objects()){
                    group->add_object(o);
                }
                return;
            }
        }
    }
}

void MainWindow::on_menuSelection_aboutToShow(){
    populateAddToGroupMenu();
}
