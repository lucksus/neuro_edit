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

    if(m_parent) m_parent->add_potential(m_added_current);
}

void DendriticNode::reset(){
    BOOST_FOREACH(DendriticNode* child, m_children){
        child->reset();
    }

    m_added_current = 0;
}

SimulationObject* DendriticNode::clone(){
    return new DendriticNode(this);
}

void DendriticNode::add_potential(double milli_volt){
    m_added_current = milli_volt;
}

double DendriticNode::added_potential(){
    return m_added_current;
}
