#include "axonnode.h"
#include <boost/foreach.hpp>
#include "axon.h"

AxonNode::AxonNode(Neuron* neuron)
    : SpatialObject(neuron), SpikeEmitter(neuron), SpikeReceiver(neuron)
{

}

void AxonNode::update(double milli_seconds){
    reset_spike_emitter();
    if(spike_received()) emit_spike();
    reset_received_spikes();

    BOOST_FOREACH(Axon* receiver, m_receivers){
        receiver->update(milli_seconds);
    }
}

//SimulationObject* AxonNode::clone(){
//    return new AxonNode(this);
//}


void AxonNode::add_receiver(Axon* axon){
    m_receivers.insert(axon);
}

void AxonNode::remove_receiver(Axon* axon){
    m_receivers.erase(axon);
}
