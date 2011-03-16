#ifndef DENDRITICNODE_H
#define DENDRITICNODE_H
#include <list>
#include "spatialobject.h"
#include "synapse.h"
#include <boost/foreach.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include "serializationhelper.h"

class DendriticNode : public SpatialObject
{
friend class boost::serialization::access;
public:
    DendriticNode(){}
    DendriticNode(Neuron* neuron, DendriticNode* parent);

    virtual void update(double milli_seconds);

    void add_current(double volt_per_second);
    double added_current();

    void reset();

    DendriticNode* parent();
    std::set<SimulationObject*> children();

    void add_incoming_synapse(Synapse*);
    void detach_incoming_synapse(Synapse*);

protected:
    virtual void moved(Point new_position);

private:
    double m_added_current;
    DendriticNode* m_parent;
    std::set<DendriticNode*> m_children;
    std::set<Synapse*> m_incoming_synapses;
/*
    template<class Archive>
    void save(Archive & ar, const unsigned int) const
    {
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpatialObject);
        ar << BOOST_SERIALIZATION_NVP(m_added_current);
        ar << BOOST_SERIALIZATION_NVP(m_parent);
        ar << BOOST_SERIALIZATION_NVP(m_children);

        std::set<Synapse*> synapses;
        BOOST_FOREACH(Synapse* synapse, m_incoming_synapses){
            if(SerializationHelper::instance().is_to_be_serialized(synapse->neuron()))
                synapses.insert(synapse);
        }

        ar << boost::serialization::make_nvp("m_incoming_synapses",synapses);
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int)
    {
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpatialObject);
        ar >> BOOST_SERIALIZATION_NVP(m_added_current);
        ar >> BOOST_SERIALIZATION_NVP(m_parent);
        ar >> BOOST_SERIALIZATION_NVP(m_children);
        ar >> boost::serialization::make_nvp("m_incoming_synapses",m_incoming_synapses);
    }
    */
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpatialObject);
        ar & BOOST_SERIALIZATION_NVP(m_added_current);
        ar & BOOST_SERIALIZATION_NVP(m_parent);
        ar & BOOST_SERIALIZATION_NVP(m_children);
        ar & BOOST_SERIALIZATION_NVP(m_incoming_synapses);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SpatialObject, DendriticNode>: public mpl::true_ {};
}

#endif // DENDRITICNODE_H
