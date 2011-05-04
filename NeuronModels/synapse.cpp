#include "synapse.h"
#include "dendriticnode.h"

Synapse::Synapse(Neuron* neuron, DendriticNode* target)
    : SpatialObject(neuron), SpikingObject(neuron), m_postsynaptic_neuron(target), m_weight(30), m_time_constant(50)
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

double Synapse::weight() const{
    return m_weight;
}
void Synapse::set_weight(double weight){
    m_weight = weight;
}

double Synapse::time_constant() const{
    return m_time_constant;
}
void Synapse::set_time_constant(double time_constant){
    m_time_constant = time_constant;
}

Neuron* Synapse::postsynaptic_neuron(){
    return m_postsynaptic_neuron->neuron();
}
