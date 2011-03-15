#include "spikereceiver.h"

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
