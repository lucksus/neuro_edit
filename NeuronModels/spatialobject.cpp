#include "spatialobject.h"

SpatialObject::SpatialObject()
{
}

SpatialObject::SpatialObject(const SpatialObject& spo)
    : SimulationObject(*this)
{
    m_position = spo.m_position;
}


Point SpatialObject::position() const{
    return m_position;
}

void SpatialObject::set_position(const Point& p){
    m_position = p;
}
