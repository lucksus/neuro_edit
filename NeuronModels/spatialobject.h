#ifndef SPATIALOBJECT_H
#define SPATIALOBJECT_H
#include "simulationobject.h"
#include "point.h"

class SpatialObject : public SimulationObject
{
public:
    Point position() const;
    void set_position(const Point& p);
private:
    Point m_position;
};

#endif // SPATIALOBJECT_H
