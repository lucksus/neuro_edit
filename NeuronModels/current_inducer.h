#ifndef CURRENT_INDUCER_H
#define CURRENT_INDUCER_H
#include "simulationobject.h"
#include <boost/serialization/access.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/nvp.hpp>
#include "dendriticnode.h"

class CurrentInducer : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(double current READ current WRITE set_current)
Q_PROPERTY(bool active READ active WRITE set_active)
friend class boost::serialization::access;
public:
    CurrentInducer():m_target(0),m_current(0),m_active(false){};
    CurrentInducer(Simulation*);
    CurrentInducer(DendriticNode*);

    virtual SimulationObject* clone();
    void set_current(double current);
    double current();
    void set_active(bool);
    bool active();

    virtual void update(double milliseconds);

private:
    DendriticNode* m_target;
    double m_current;
    bool m_active;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("target", m_target);
        ar & boost::serialization::make_nvp("current", m_current);
        ar & boost::serialization::make_nvp("active", m_active);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, CurrentInducer>: public mpl::true_ {};
}

Q_DECLARE_METATYPE(CurrentInducer)
Q_DECLARE_METATYPE(CurrentInducer*)

#endif // CURRENT_INDUCER_H
