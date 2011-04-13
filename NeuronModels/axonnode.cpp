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
    done();
}

//SimulationObject* AxonNode::clone(){
//    return new AxonNode(this);
//}


void AxonNode::add_receiving_axon(Axon* axon){
    m_receivers.insert(axon);
}

void AxonNode::remove_receiving_axon(Axon* axon){
    m_receivers.erase(axon);
}

std::set<Axon*> AxonNode::receiving_axons(){
    return m_receivers;
}


std::set<SimulationObject*> AxonNode::about_to_remove(SimulationObject *object_to_be_deleted){
    Axon* axon = dynamic_cast<Axon*>(object_to_be_deleted);
    if(axon){
        m_receivers.erase(axon);
    }
    return SimulationObject::about_to_remove(object_to_be_deleted);
}
