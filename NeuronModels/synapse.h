#ifndef SYNAPSE_H
#define SYNAPSE_H
#include <list>
#include "spikereceiver.h"
#include "editableobject.h"
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

class DendriticNode;
class Axon;
class Synapse : public SpikeReceiver, public virtual EditableObject
{
friend class boost::serialization::access;
public:
    Synapse(Neuron* neuron, DendriticNode* target);
    virtual ~Synapse();

    virtual void update(double milli_seconds);

    virtual Properties properties();
    virtual void set_property(std::string group, std::string name, boost::any value);

    Neuron* postsynaptic_neuron();

private:
    DendriticNode* m_postsynaptic_neuron;
    double m_weight;
    std::list<double> m_active_potentials;

    Synapse() {}

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<SpikeReceiver>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikeReceiver);
        ar & boost::serialization::make_nvp("PostsynapticDendriticNode", m_postsynaptic_neuron);
        ar & boost::serialization::make_nvp("Weight", m_weight);
        ar & boost::serialization::make_nvp("ActivePotentials", m_active_potentials);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SpikeReceiver, Synapse>: public mpl::true_ {};
}


#endif // SYNAPSE_H
