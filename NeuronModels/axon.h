#ifndef LINK_H
#define LINK_H
#include "simulationobject.h"
#include <list>
#include "editableobject.h"
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

class Neuron;
class SpikingObject;
class Axon : public SimulationObject, public EditableObject
{
friend class boost::serialization::access;
public:
    Axon(Neuron* neuron, SpikingObject* emitter, SpikingObject* receiver, double speed=5);
    virtual ~Axon();

    virtual void update(double milli_seconds);

    double speed();
    void set_speed(double speed);
    std::list<double> action_potentials_normalized();

    void set_emitter(SpikingObject* emitter);
    SpikingObject* emitter();
    SpikingObject* receiver();

    virtual Properties properties();
    virtual void set_property(std::string group, std::string name, boost::any value);

    virtual std::set<SimulationObject*> about_to_remove(SimulationObject *);

private:
    SpikingObject* m_emitter;
    SpikingObject* m_receiver;
    double m_speed; //mm per ms
    double m_runtime; //cached
    std::list<double> m_action_potentials;

    void update_runtime();

    Axon() {}
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("Emitter", m_emitter);
        ar & boost::serialization::make_nvp("Receiver", m_receiver);
        ar & boost::serialization::make_nvp("Speed", m_speed);
        ar & boost::serialization::make_nvp("ActionPotentials", m_action_potentials);
        update_runtime();
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, Axon>: public mpl::true_ {};
}

#endif // LINK_H
