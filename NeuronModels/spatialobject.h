#ifndef SPATIALOBJECT_H
#define SPATIALOBJECT_H
#include "simulationobject.h"
#include "point.h"
#include "editableobject.h"

class SpatialObject : public SimulationObject , public virtual EditableObject
{
public:
    SpatialObject();
    SpatialObject(const SpatialObject&);

    Point position() const;
    void set_position(const Point& p);

    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);
private:
    Point m_position;
};

#endif // SPATIALOBJECT_H
