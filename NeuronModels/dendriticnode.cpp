#include "dendriticnode.h"
#include <boost/foreach.hpp>

DendriticNode::DendriticNode(DendriticNode* parent)
    : m_parent(parent)
{
}


void DendriticNode::update(double milli_seconds){
    BOOST_FOREACH(DendriticNode* child, m_children){
        child->update(milli_seconds);
    }

    if(m_parent){
        m_parent->add_current(m_added_current);
        m_added_current = 0;
    }
}

void DendriticNode::reset(){
    m_added_current = 0;
}

SimulationObject* DendriticNode::clone(){
    return new DendriticNode(this);
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
