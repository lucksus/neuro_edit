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
