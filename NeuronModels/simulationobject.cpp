#include "simulationobject.h"
#include <boost/foreach.hpp>
#include "neuron.h"

SimulationObject::SimulationObject(Simulation* s)
    : m_network(0), m_neuron(0), m_done(false), m_is_user_movable(true), m_simulation(s)
{
    bad_hacks[0] = bad_hacks[1] = bad_hacks[2] = 0;
}

SimulationObject::SimulationObject(Neuron* neuron)
    : m_network(0), m_neuron(neuron), m_done(false), m_is_user_movable(true), m_simulation(neuron->m_simulation)
{
    bad_hacks[0] = bad_hacks[1] = bad_hacks[2] = 0;
}

SimulationObject::SimulationObject(const SimulationObject& s)
    : QObject(), m_network(s.m_network), m_neuron(s.m_neuron), m_done(s.m_done), m_is_user_movable(true), m_simulation(s.m_simulation)
{
    bad_hacks[0] = bad_hacks[1] = bad_hacks[2] = 0;
}


std::set<SimulationObject*> SimulationObject::children(){
    return std::set<SimulationObject*>();
}


Neuron* SimulationObject::neuron(){
    return m_neuron;
}

Simulation* SimulationObject::simulation(){
    return m_simulation;
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

Point SimulationObject::position() const{
    return m_position;
}

void SimulationObject::set_position(const Point& p){
    moved(p);
    m_position = p;
}

void SimulationObject::moved(Point){
}

bool SimulationObject::is_user_movable(){
    return m_is_user_movable;
}

void SimulationObject::set_user_movable(bool user_movable){
    m_is_user_movable = user_movable;
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
