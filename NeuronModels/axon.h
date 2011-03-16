#ifndef LINK_H
#define LINK_H
#include "simulationobject.h"
#include <list>
#include "spikeemitter.h"
#include "spikereceiver.h"
#include "editableobject.h"
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

class Neuron;
class Axon : public SimulationObject, public EditableObject
{
friend class boost::serialization::access;
public:
    Axon(Neuron* neuron, SpikeEmitter* emitter, SpikeReceiver* receiver, double speed=5);

    virtual void update(double milli_seconds);

    double speed();
    void set_speed(double speed);
    std::list<double> action_potentials_normalized();

    void set_emitter(SpikeEmitter* emitter);
    SpikeEmitter* emitter();
    SpikeReceiver* receiver();

    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);

private:
    SpikeEmitter* m_emitter;
    SpikeReceiver* m_receiver;
    double m_speed; //mm per ms
    double m_runtime; //cached
    std::list<double> m_action_potentials;

    Axon() {}
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & BOOST_SERIALIZATION_NVP(m_emitter);
        ar & BOOST_SERIALIZATION_NVP(m_receiver);
        ar & BOOST_SERIALIZATION_NVP(m_speed);
        ar & BOOST_SERIALIZATION_NVP(m_action_potentials);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, Axon>: public mpl::true_ {};
}

#endif // LINK_H
