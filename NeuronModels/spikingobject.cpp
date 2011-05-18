#include "spikingobject.h"
#include "axon.h"

SpikingObject::SpikingObject(Neuron* neuron)
: SimulationObject(neuron), m_spiking(false), m_spike_received(false)
{
}

void SpikingObject::reset_spike_emitter(){
    m_spiking = false;
}

bool SpikingObject::is_spiking(){
    return m_spiking;
}

void SpikingObject::emit_spike(){
    m_spiking = true;
}


void SpikingObject::receive_spike(){
    m_spike_received = true;
}

void SpikingObject::reset_received_spikes(){
    m_spike_received = false;
}


bool SpikingObject::spike_received(){
    return m_spike_received;
}

void SpikingObject::add_incoming_axon(Axon* axon){
    m_incoming_axons.insert(axon);
}

std::set<Axon*> SpikingObject::incoming_axons(){
    return m_incoming_axons;
}

std::set<SimulationObject*> SpikingObject::about_to_remove(SimulationObject *object_to_be_deleted){
    Axon* axon = dynamic_cast<Axon*>(object_to_be_deleted);
    if(axon)
        m_incoming_axons.erase(axon);
    return SimulationObject::about_to_remove(object_to_be_deleted);
}
