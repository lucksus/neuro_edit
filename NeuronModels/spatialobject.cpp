#include "spatialobject.h"

Point SpatialObject::position() const{
    return m_position;
}

void SpatialObject::set_position(const Point& p){
    m_position = p;
}
