#ifndef LINK_H
#define LINK_H
#include "simulationobject.h"
#include <list>
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <QtCore/QMetaType>

class Neuron;
class SpikingObject;
class Axon : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(double speed READ speed WRITE set_speed)
friend class boost::serialization::access;
public:
    Axon() {}
    Axon(Neuron* neuron, SpikingObject* emitter, SpikingObject* receiver, double speed=5);
    virtual ~Axon();

    virtual void update(double milli_seconds);
    Q_INVOKABLE virtual void reset();

    double speed();
    void set_speed(double speed);
    std::list<double> action_potentials_normalized();

    void set_emitter(SpikingObject* emitter);
    SpikingObject* emitter();
    SpikingObject* receiver();

    virtual std::set<SimulationObject*> about_to_remove(SimulationObject *);

private:
    SpikingObject* m_emitter;
    SpikingObject* m_receiver;
    double m_speed; //mm per ms
    double m_runtime; //cached
    std::list<double> m_action_potentials;

    void update_runtime();

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

Q_DECLARE_METATYPE(Axon)
#endif // LINK_H
