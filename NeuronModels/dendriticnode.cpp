#include "dendriticnode.h"
#include <boost/foreach.hpp>
#include "synapse.h"
#include "network.h"
#include "current_inducer.h"

DendriticNode::DendriticNode(Neuron* neuron, DendriticNode* parent)
    : SimulationObject(neuron), m_added_current(0), m_parent(parent)
{
    if(parent)
        parent->add_child(this);
}


void DendriticNode::update(double milli_seconds){
    BOOST_FOREACH(DendriticNode* child, m_children){
        child->update(milli_seconds);
    }

    if(m_parent){
        m_parent->add_current(m_added_current);
        m_added_current = 0;
    }
    done();
}

void DendriticNode::reset(){
    m_added_current = 0;
}


void DendriticNode::add_current(double milli_volt){
    m_added_current = milli_volt;
}

double DendriticNode::added_current(){
    return m_added_current;
}

DendriticNode* DendriticNode::parent(){
    return m_parent;
}

std::set<SimulationObject*> DendriticNode::children(){
    std::set<SimulationObject*> result;
    BOOST_FOREACH(DendriticNode* node, m_children){
        result.insert(node);
    }

    return result;
}

void DendriticNode::moved(Point new_position){
    BOOST_FOREACH(Synapse* synapse, m_incoming_synapses){
        synapse->set_position(new_position);
    }
    BOOST_FOREACH(CurrentInducer* ci, m_current_inducers){
        ci->set_position(new_position);
    }
}


void DendriticNode::add_incoming_synapse(Synapse* synapse){
    m_incoming_synapses.insert(synapse);
}

void DendriticNode::detach_incoming_synapse(Synapse* synapse){
    m_incoming_synapses.erase(synapse);
}

void DendriticNode::add_current_inducer(CurrentInducer* ci){
    m_current_inducers.insert(ci);
}

void DendriticNode::detach_current_inducer(CurrentInducer* ci){
    m_current_inducers.erase(ci);
}

void DendriticNode::add_child(DendriticNode* child){
    m_children.insert(child);
}

std::set<SimulationObject*> DendriticNode::about_to_remove(SimulationObject *object_to_be_deleted){
    DendriticNode* dendritic_node = dynamic_cast<DendriticNode*>(object_to_be_deleted);
    Synapse* synapse = dynamic_cast<Synapse*>(object_to_be_deleted);
    if(dendritic_node)
        m_children.erase(dendritic_node);
    if(synapse)
        m_incoming_synapses.erase(synapse);

    std::set<SimulationObject*> also_to_be_deleted = SimulationObject::about_to_remove(object_to_be_deleted);
    if(this == object_to_be_deleted){
        BOOST_FOREACH(DendriticNode* n, m_children){
            also_to_be_deleted.insert(n);
        }
    }

    return also_to_be_deleted;
}






