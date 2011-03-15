#include "synapse.h"

Synapse::Synapse(DendriticNode* target)
    : m_postsynaptic_neuron(target)
{
}

Synapse::~Synapse(){

}

void Synapse::update(double milli_seconds){

}

std::map<std::string, boost::any> Synapse::properties(){
    std::map<std::string, boost::any> result;
    result["weight"] = m_weight;
    return result;
}

void Synapse::set_property(std::string name, boost::any value){
    if("weight" == name) m_weight = boost::any_cast<double>(value);
}
