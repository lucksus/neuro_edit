#include "simulationobject.h"
#include <boost/foreach.hpp>
#include "neuron.h"
#include "log.h"

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

void SimulationObject::set_neuron(Neuron* neuron){
    m_neuron = neuron;
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
    moved(p,m_position);
    m_position = p;
}

Point* SimulationObject::pos(){
    return &m_position;
}

void SimulationObject::set_pos(Point* p){
    m_position = *p;
}


void SimulationObject::moved(Point){
}

void SimulationObject::moved(Point, Point){
}

bool SimulationObject::is_user_movable(){
    return m_is_user_movable;
}

void SimulationObject::set_user_movable(bool user_movable){
    m_is_user_movable = user_movable;
}

Point SimulationObject::moving_offset() const{
    return Point();
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

std::set<std::string> SimulationObject::active_user_actions() {
    std::set<std::string> s;
    BOOST_FOREACH(std::string action, user_actions()){
        s.insert(action);
    }
    return s;
}

void SimulationObject::log(std::string message){
    Log::instance().log(message, this);
}

void SimulationObject::debug(std::string message){
    Log::instance().log(message, this, Log::DEBUG_MSG);
}

void SimulationObject::error(std::string message){
    Log::instance().log(message, this, Log::ERROR_MSG);
}

