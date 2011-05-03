#ifndef SYNAPSE_H
#define SYNAPSE_H
#include <list>
#include "spikingobject.h"
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

class DendriticNode;
class Axon;
class Synapse : public SpikingObject
{
Q_OBJECT
Q_PROPERTY(double weight READ weight WRITE set_weight)
Q_PROPERTY(double time_constant READ time_constant WRITE set_time_constant)
Q_PROPERTY(Neuron* postsynaptic_neuron READ postsynaptic_neuron)
friend class boost::serialization::access;
public:
    Synapse() {}
    Synapse(Neuron* neuron, DendriticNode* target);
    virtual ~Synapse();
    virtual void update(double milli_seconds);

    double weight() const;
    void set_weight(double weight);
    double time_constant() const;
    void set_time_constant(double time_constant);
    Neuron* postsynaptic_neuron();

private:
    DendriticNode* m_postsynaptic_neuron;
    double m_weight;
    double m_time_constant;
    std::list<double> m_active_potentials;



    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<SpikeReceiver>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikingObject);
        ar & boost::serialization::make_nvp("PostsynapticDendriticNode", m_postsynaptic_neuron);
        ar & boost::serialization::make_nvp("Weight", m_weight);
        ar & boost::serialization::make_nvp("Time_constant", m_time_constant);
        ar & boost::serialization::make_nvp("ActivePotentials", m_active_potentials);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SpikingObject, Synapse>: public mpl::true_ {};
}

Q_DECLARE_METATYPE(Synapse)
#endif // SYNAPSE_H
