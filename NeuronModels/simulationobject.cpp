#include "simulationobject.h"
#include <boost/foreach.hpp>s

SimulationObject::SimulationObject(Neuron* neuron)
    : m_network(0), m_neuron(neuron), m_done(false)
{

}


std::set<SimulationObject*> SimulationObject::children(){
    return std::set<SimulationObject*>();
}


Neuron* SimulationObject::neuron(){
    return m_neuron;
}

bool SimulationObject::is_done(){
    return m_done;
}

void SimulationObject::reset_done(){
    m_done = false;
}

void SimulationObject::done(){
    m_done = true;
}

void SimulationObject::set_network(Network* network){
    m_network = network;
}


std::set<SimulationObject*> SimulationObject::about_to_remove(SimulationObject* object_to_be_deleted){
    if(this == object_to_be_deleted){
        std::set<SimulationObject*> also_to_be_deleted;
        BOOST_FOREACH(SimulationObject* child, children()){
            std::set<SimulationObject*> child_also_to_be_deleted = child->about_to_remove(object_to_be_deleted);
            BOOST_FOREACH(SimulationObject* o, child_also_to_be_deleted){
                also_to_be_deleted.insert(o);
            }
            also_to_be_deleted.insert(child);
        }
        return also_to_be_deleted;
    }

    return std::set<SimulationObject*>();
}
