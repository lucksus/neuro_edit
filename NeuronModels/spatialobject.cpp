#include "spatialobject.h"

SpatialObject::SpatialObject()
{
}

SpatialObject::SpatialObject(const SpatialObject& spo)
    : EditableObject(spo), SimulationObject(spo)
{
    m_position = spo.m_position;
}


Point SpatialObject::position() const{
    return m_position;
}

void SpatialObject::set_position(const Point& p){
    m_position = p;
}


std::map<std::string, boost::any> SpatialObject::properties(){
    std::map<std::string, boost::any> result;
    result["position x"] = m_position.x;
    result["position y"] = m_position.y;
    result["position z"] = m_position.z;
    return result;
}

void SpatialObject::set_property(std::string name, boost::any value){
    if("position x" == name) m_position.x = boost::any_cast<double>(value);
    if("position y" == name) m_position.y = boost::any_cast<double>(value);
    if("position z" == name) m_position.z = boost::any_cast<double>(value);
}
