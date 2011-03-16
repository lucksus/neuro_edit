#ifndef SPIKENODE_H
#define SPIKENODE_H
#include "spikeemitter.h"
#include "spikereceiver.h"
#include "spatialobject.h"
#include <set>
#include <boost/foreach.hpp>
#include "axon.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include "serializationhelper.h"

class AxonNode : public SpikeEmitter, public SpikeReceiver
{
friend class boost::serialization::access;
public:
    AxonNode() {}
    AxonNode(Neuron*);

    virtual void update(double milli_seconds);
    //virtual SimulationObject* clone();

    void add_receiver(Axon*);
    void remove_receiver(Axon*);

private:
    std::set<Axon*> m_receivers;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikeEmitter);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikeReceiver);
        ar & boost::serialization::make_nvp("m_receivers",m_receivers);
    }
/*
    template<class Archive>
    void save(Archive & ar, const unsigned int) const
    {
        ar << boost::serialization::base_object<SpikeEmitter>(*this);
        ar << boost::serialization::base_object<SpikeReceiver>(*this);

        std::set<Axon*> axons;
        BOOST_FOREACH(Axon* axon, m_receivers){
            if(SerializationHelper::instance().is_to_be_serialized(axon->receiver()->neuron()))
                axons.insert(axon);
        }

        ar << boost::serialization::make_nvp("m_receivers",axons);
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int)
    {
        ar >> boost::serialization::base_object<SpikeEmitter>(*this);
        ar >> boost::serialization::base_object<SpikeReceiver>(*this);
        ar >> boost::serialization::make_nvp("m_receivers",m_receivers);
    }*/
};

namespace boost{
template<>
struct is_virtual_base_of<SpikeEmitter, AxonNode>: public mpl::true_ {};
//template<>
//struct is_virtual_base_of<SpikeReceiver, AxonNode>: public mpl::true_ {};
}

#endif // SPIKENODE_H
