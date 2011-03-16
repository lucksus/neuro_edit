#include "synapse.h"
#include "dendriticnode.h"

Synapse::Synapse(Neuron* neuron, DendriticNode* target)
    : SpatialObject(neuron), SpikeReceiver(neuron), m_postsynaptic_neuron(target)
{
    set_position(target->position());
    set_user_movable(false);
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
