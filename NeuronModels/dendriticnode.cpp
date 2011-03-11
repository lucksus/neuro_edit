#include "dendriticnode.h"

DendriticNode::DendriticNode(DendriticNode* parent)
    : m_parent(parent)
{
}


void DendriticNode::update(double milli_seconds){
    if(m_parent) m_parent->add_potential(m_added_potential);
}

SimulationObject* DendriticNode::clone(){
    return new DendriticNode(this);
}

void DendriticNode::add_potential(double milli_volt){
    m_added_potential = milli_volt;
}


double DendriticNode::added_potential(){
    return m_added_potential;
}
