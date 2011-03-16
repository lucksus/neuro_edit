#ifndef SPATIALOBJECT_H
#define SPATIALOBJECT_H
#include "simulationobject.h"
#include "point.h"
#include "editableobject.h"

class SpatialObject : public SimulationObject , public virtual EditableObject
{
public:
    SpatialObject(Neuron*);
    SpatialObject(const SpatialObject&);

    Point position() const;
    void set_position(const Point& p);

    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);

    virtual bool is_user_movable();
    void set_user_movable(bool user_movable);

protected:
    virtual void moved(Point new_position);

private:
    Point m_position;
    bool m_is_user_movable;
};

#endif // SPATIALOBJECT_H
