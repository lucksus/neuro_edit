#include "dendriticnode.h"
#include <boost/foreach.hpp>
#include "synapse.h"

DendriticNode::DendriticNode(Neuron* neuron, DendriticNode* parent)
    : SpatialObject(neuron), m_parent(parent), m_added_current(0)
{
    if(parent)
        parent->add_child(this);
}


void DendriticNode::update(double milli_seconds){
    BOOST_FOREACH(DendriticNode* child, m_children){
        if(!child->is_done())
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
}


void DendriticNode::add_incoming_synapse(Synapse* synapse){
    m_incoming_synapses.insert(synapse);
}

void DendriticNode::detach_incoming_synapse(Synapse* synapse){
    m_incoming_synapses.erase(synapse);
}

void DendriticNode::add_child(DendriticNode* child){
    m_children.insert(child);
}
