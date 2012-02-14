#ifndef DENDRITICNODE_H
#define DENDRITICNODE_H
#include <list>
#include "simulationobject.h"
#include "synapse.h"
#include <boost/foreach.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>

class LinearDiscriminator;
class CurrentInducer;

class DendriticNode : public SimulationObject
{
Q_OBJECT
friend class boost::serialization::access;
public:
    DendriticNode(){}
    DendriticNode(Neuron* neuron, DendriticNode* parent);
    DendriticNode(LinearDiscriminator*);

    virtual void update(double milli_seconds);

    void add_current(double volt_per_second);
    double added_current();

    void reset();

    DendriticNode* parent();
    std::set<SimulationObject*> children();

    void add_incoming_synapse(Synapse*);
    void detach_incoming_synapse(Synapse*);
    void add_current_inducer(CurrentInducer*);
    void detach_current_inducer(CurrentInducer*);
    void add_child(DendriticNode*);

    virtual std::set<SimulationObject*> about_to_remove(SimulationObject *);

    std::set<Synapse*> incoming_synapses();

protected:
    virtual void moved(Point new_position);

private:
    double m_added_current;
    DendriticNode* m_parent;
    LinearDiscriminator* m_linear_discriminator;
    std::set<DendriticNode*> m_children;
    std::set<Synapse*> m_incoming_synapses;
    std::set<CurrentInducer*> m_current_inducers;
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
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("AddedCurrent", m_added_current);
        ar & boost::serialization::make_nvp("Parent", m_parent);
        ar & boost::serialization::make_nvp("Children", m_children);
        ar & boost::serialization::make_nvp("IncomingSynapses", m_incoming_synapses);
        ar & boost::serialization::make_nvp("AttachedCurrentInducers", m_current_inducers);
        ar & boost::serialization::make_nvp("LinearDiscriminator", m_linear_discriminator);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, DendriticNode>: public mpl::true_ {};
}

Q_DECLARE_METATYPE(DendriticNode)
Q_DECLARE_METATYPE(DendriticNode*)
#endif // DENDRITICNODE_H
