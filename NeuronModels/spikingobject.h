#ifndef SPIKINGOBJECT_H
#define SPIKINGOBJECT_H
#include "spatialobject.h"
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>

class Axon;
class SpikingObject : public virtual SpatialObject
{
friend class boost::serialization::access;
public:
    SpikingObject(Neuron*);

    void receive_spike();
    void add_incoming_axon(Axon*);
    std::set<Axon*> incoming_axons();
    bool is_spiking();

    virtual std::set<SimulationObject*> about_to_remove(SimulationObject *);

protected:
    SpikingObject(){}
    void emit_spike();
    void reset_spike_emitter();
    bool spike_received();
    void reset_received_spikes();

private:
    bool m_spiking;
    bool m_spike_received;
    std::set<Axon*> m_incoming_axons;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<SpatialObject>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpatialObject);
        ar & boost::serialization::make_nvp("spike_received", m_spike_received);
        ar & boost::serialization::make_nvp("IncomingAxons", m_incoming_axons);
        ar & boost::serialization::make_nvp("is_spiking", m_spiking);
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(SpikingObject);

namespace boost{
template<>
struct is_virtual_base_of<SpatialObject, SpikingObject>: public mpl::true_ {};
}

#endif // SPIKINGOBJECT_H
