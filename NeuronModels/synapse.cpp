#include "synapse.h"
#include "dendriticnode.h"

Synapse::Synapse(Neuron* neuron, DendriticNode* target)
    : SpikingObject(neuron), m_postsynaptic_neuron(target), m_weight(30), m_time_constant(50)
{
    set_position(target->position());
    set_user_movable(false);
}

Synapse::~Synapse(){

}

void Synapse::update(double milli_seconds){
    if(spike_received()){
        m_active_potentials.push_back(1);
    }
    reset_received_spikes();

    while(m_active_potentials.size() > 0 && m_active_potentials.front() < 0.01)
        m_active_potentials.pop_front();

    BOOST_FOREACH(double potential, m_active_potentials){
        m_postsynaptic_neuron->add_current(m_weight * potential);
    }

    BOOST_FOREACH(double &potential, m_active_potentials){
        potential -= potential / 100 * m_time_constant * milli_seconds;
    }

}

Properties Synapse::properties(){
    Properties properties = SpikingObject::properties();
    properties.set_group("Synapse");
    properties.add("weight", m_weight);
    properties.set_description("weight", "Maximum current induced in postsynaptic neuron by one potential.");
    properties.set_unit("weight", "mV/ms");
    properties.add("time constant", m_time_constant);
    properties.set_description("time constant", "Part of potential that is substracted from potential every millisecond.");
    properties.set_unit("time constant", "%");
    return properties;
}

void Synapse::set_property(std::string group, std::string name, boost::any value){
    if("weight" == name && "Synapse" == group) m_weight = boost::any_cast<double>(value);
    if("time constant" == name && "Synapse" == group) m_time_constant = boost::any_cast<double>(value);
}

Neuron* Synapse::postsynaptic_neuron(){
    return m_postsynaptic_neuron->neuron();
}
