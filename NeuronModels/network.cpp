#include "network.h"
#include <boost/foreach.hpp>

void Network::add_object(SimulationObject* object){
    m_objects.push_back(object);
}

void Network::delete_object(SimulationObject* object){
    m_objects.remove(object);
    delete object;
}

std::list<SimulationObject*> Network::objects(){
    return m_objects;
}

void Network::simulate(double milli_seconds){
    BOOST_FOREACH(SimulationObject* o, m_objects){
        o->update(milli_seconds);
    }
}
