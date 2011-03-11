#include "spikenode.h"

void SpikeNode::update(double milli_seconds){
    reset_spike_emitter();
    if(spike_received()) emit_spike();
    reset_received_spikes();
}

SimulationObject* SpikeNode::clone(){
    return new SpikeNode(this);
}
