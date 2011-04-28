#include "synapse.h"
#include "dendriticnode.h"

Synapse::Synapse(Neuron* neuron, DendriticNode* target)
    : SpatialObject(neuron), SpikeReceiver(neuron), m_postsynaptic_neuron(target), m_weight(0)
{
    set_position(target->position());
    set_user_movable(false);
}

Synapse::~Synapse(){

}

void Synapse::update(double milli_seconds){

}

Properties Synapse::properties(){
    Properties properties = SpikeReceiver::properties();
    properties.set_group("Synapse");
    properties.add("weight", m_weight);
    return properties;
}

void Synapse::set_property(std::string group, std::string name, boost::any value){
    if("weight" == name && "Synapse" == group) m_weight = boost::any_cast<double>(value);
}

Neuron* Synapse::postsynaptic_neuron(){
    return m_postsynaptic_neuron->neuron();
}
