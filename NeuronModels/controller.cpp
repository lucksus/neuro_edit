#include "controller.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QMetaType>
#include "point.h"
#include "axon.h"
#include "axonnode.h"
#include "dendriticnode.h"
#include "izhikevich.h"
#include "network.h"
#include "neuron.h"
#include "neuronmodel.h"
#include "synapse.h"
#include "simulation.h"
#include <stdexcept>
#include "samples.h"

Controller::Controller()
{
}

Controller& Controller::instance(){
    static Controller c;
    return c;
}

void Controller::init(){
    QCoreApplication::setOrganizationName("lucksus");
    QCoreApplication::setOrganizationDomain("lucksus.eu");
    QCoreApplication::setApplicationName("neuro_edit");

    qRegisterMetaType<Point>("Point");
    qRegisterMetaType<Axon>("Axon");
    qRegisterMetaType<AxonNode>("AxonNode");
    qRegisterMetaType<DendriticNode>("DendriticNode");
    qRegisterMetaType<Izhikevich>("Izhikevich");
    qRegisterMetaType<Network>("Network");
    qRegisterMetaType<Network*>("Network*");
    qRegisterMetaType<Neuron>("Neuron");
    qRegisterMetaType<Neuron*>("Neuron*");
    qRegisterMetaType<NeuronModel*>("NeuronModel*");
    qRegisterMetaType<Synapse>("Synapse");
    qRegisterMetaType<Samples*>("Samples*");
}

void Controller::close_simulation(){
    if(0 == m_simulation) return;
    m_simulation->request_stop();
    m_simulation->wait_till_finished();
    delete m_simulation;
    m_simulation = 0;
    emit new_simulation(0);
}

void Controller::create_new_simulation(){
    close_simulation();
    m_simulation = new Simulation;
    m_simulation->set_network(new Network(m_simulation));
    emit new_simulation(m_simulation);
}

void Controller::load_simulation(std::string filename){
    close_simulation();
    m_simulation = Simulation::load_from_file(filename, Simulation::BINARY);
    m_simulation_filename = filename;
    emit new_simulation(m_simulation);
}

void Controller::save_simulation(std::string filename){
    m_simulation->write_to_file(filename, Simulation::BINARY);
    m_simulation_filename = filename;
}

void Controller::export_xml_simulation(std::string filename){
    m_simulation->write_to_file(filename, Simulation::XML);
}

void Controller::import_xml_simulation(std::string filename){
    close_simulation();
    m_simulation = Simulation::load_from_file(filename, Simulation::XML);
    emit new_simulation(m_simulation);
}

void Controller::save_simulation(){
    static std::string error_message = "simulation filename not set!";
    if(m_simulation_filename == "") throw std::logic_error(error_message);
    save_simulation(m_simulation_filename);
}

Simulation* Controller::simulation(){
    return m_simulation;
}

void Controller::output_from_script(QString output){
    emit script_output(output);
}
