#ifndef SPATIALOBJECT_H
#define SPATIALOBJECT_H
#include "simulationobject.h"
#include "point.h"
#include "editableobject.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

class SpatialObject : public SimulationObject , public virtual EditableObject
{
friend class boost::serialization::access;
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
    SpatialObject() {}

private:
    Point m_position;
    bool m_is_user_movable;


    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<SimulationObject>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & BOOST_SERIALIZATION_NVP(m_position);
        ar & BOOST_SERIALIZATION_NVP(m_is_user_movable);
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(SpatialObject);

namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, SpatialObject>: public mpl::true_ {};
}
#endif // SPATIALOBJECT_H
