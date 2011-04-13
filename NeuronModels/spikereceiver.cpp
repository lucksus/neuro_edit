#include "spikereceiver.h"
#include "axon.h"

SpikeReceiver::SpikeReceiver(Neuron* neuron)
    : SpatialObject(neuron), m_spike_received(false)
{
}

void SpikeReceiver::receive_spike(){
    m_spike_received = true;
}

void SpikeReceiver::reset_received_spikes(){
    m_spike_received = false;
}


bool SpikeReceiver::spike_received(){
    return m_spike_received;
}

void SpikeReceiver::add_incoming_axon(Axon* axon){
    m_incoming_axons.insert(axon);
}

std::set<Axon*> SpikeReceiver::incoming_axons(){
    return m_incoming_axons;
}

std::set<SimulationObject*> SpikeReceiver::about_to_remove(SimulationObject *object_to_be_deleted){
    Axon* axon = dynamic_cast<Axon*>(object_to_be_deleted);
    if(axon)
        m_incoming_axons.erase(axon);
    return SimulationObject::about_to_remove(object_to_be_deleted);
}
