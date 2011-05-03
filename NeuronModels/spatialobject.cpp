#include "spatialobject.h"

SpatialObject::SpatialObject(Neuron* neuron)
    : SimulationObject(neuron), m_is_user_movable(true)
{
}

SpatialObject::SpatialObject(const SpatialObject& spo)
    : SimulationObject(spo), m_is_user_movable(true)
{
    m_position = spo.m_position;
}


Point SpatialObject::position() const{
    return m_position;
}

void SpatialObject::set_position(const Point& p){
    moved(p);
    m_position = p;
}

void SpatialObject::moved(Point){
}

bool SpatialObject::is_user_movable(){
    return m_is_user_movable;
}

void SpatialObject::set_user_movable(bool user_movable){
    m_is_user_movable = user_movable;
}
