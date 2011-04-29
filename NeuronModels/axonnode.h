#ifndef SPIKENODE_H
#define SPIKENODE_H
#include "spikingobject.h"
#include "spatialobject.h"
#include <set>
#include <boost/foreach.hpp>
#include "axon.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/archive/detail/basic_oarchive.hpp>
#include "serializationhelper.h"
#include "synapse.h"

class AxonNode : public SpikingObject
{
friend class boost::serialization::access;
public:
    AxonNode() {}
    AxonNode(Neuron*);

    virtual void update(double milli_seconds);
    //virtual SimulationObject* clone();

    void add_receiving_axon(Axon*);
    void remove_receiving_axon(Axon*);
    std::set<Axon*> receiving_axons();

    virtual std::set<SimulationObject*> about_to_remove(SimulationObject *);

private:
    std::set<Axon*> m_receivers;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikingObject);

        try{
            //this cast fails and throws std::bad_cast if ar is an iarchive.
            //so this block gets executed when saving,
            //the catch block when loading.
            dynamic_cast<boost::archive::detail::basic_oarchive&>(ar);
            //----------
            //---SAVE:--
            //----------
            std::set<Axon*> axons;
            BOOST_FOREACH(Axon* axon, m_receivers){
                SpikingObject* receiver = axon->receiver();
                Synapse* synapse = dynamic_cast<Synapse*>(receiver);
                if(synapse)
                    if(!SerializationHelper::instance().is_to_be_serialized(synapse->postsynaptic_neuron()))
                        continue;
                axons.insert(axon);
            }

            ar & boost::serialization::make_nvp("Receivers",axons);
        }catch(std::bad_cast){
            //----------
            //---LOAD:--
            //----------
            ar & boost::serialization::make_nvp("Receivers",m_receivers);
        }
    }
/*
    template<class Archive>
    void save(Archive & ar, const unsigned int) const
    {
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikeEmitter);
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikeReceiver);

        std::set<Axon*> axons;
        BOOST_FOREACH(Axon* axon, m_receivers){
            SpikeReceiver* receiver = axon->receiver();
            Synapse* synapse = dynamic_cast<Synapse*>(receiver);
            if(synapse)
                if(!SerializationHelper::instance().is_to_be_serialized(synapse->postsynaptic_neuron()))
                    continue;
            axons.insert(axon);
        }

        ar << boost::serialization::make_nvp("Receivers",axons);
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int)
    {
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikeEmitter);
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpikeReceiver);
        ar >> boost::serialization::make_nvp("Receivers",m_receivers);
    }*/
};

namespace boost{
template<>
struct is_virtual_base_of<SpikingObject, AxonNode>: public mpl::true_ {};
}

#endif // SPIKENODE_H
