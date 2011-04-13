#ifndef SPIKERECEIVER_H
#define SPIKERECEIVER_H
#include "spatialobject.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

class Axon;
class SpikeReceiver : public virtual SpatialObject
{
friend class boost::serialization::access;
public:
    SpikeReceiver(Neuron*);

    void receive_spike();

    void add_incoming_axon(Axon*);
    std::set<Axon*> incoming_axons();

    virtual std::set<SimulationObject*> about_to_remove(SimulationObject *);

protected:
    SpikeReceiver(){}
    bool spike_received();
    void reset_received_spikes();

private:
    bool m_spike_received;
    std::set<Axon*> m_incoming_axons;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<SpatialObject>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpatialObject);
        ar & boost::serialization::make_nvp("spike_received", m_spike_received);
        ar & boost::serialization::make_nvp("IncomingAxons", m_incoming_axons);
    }

};


BOOST_SERIALIZATION_ASSUME_ABSTRACT(SpikeReceiver);

namespace boost{
template<>
struct is_virtual_base_of<SpatialObject, SpikeReceiver>: public mpl::true_ {};
}

#endif // SPIKERECEIVER_H
