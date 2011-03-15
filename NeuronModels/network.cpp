#include "network.h"
#include <boost/foreach.hpp>
#include "synapse.h"
#include "axon.h"
#include <algorithm>
#include <assert.h>
#include "dendriticnode.h"

void Network::add_object(SimulationObject* object){
    m_objects.push_back(object);
    emit object_added(object);
}

void Network::delete_object(SimulationObject* object){
    m_objects.remove(object);
    delete object;
    emit object_deleted(object);
}

std::list<SimulationObject*> Network::objects(){
    return m_objects;
}

void Network::simulate(double milli_seconds){
    BOOST_FOREACH(SimulationObject* o, m_objects){
        o->update(milli_seconds);
    }
}


Axon* Network::connect(SpikeEmitter* emitter, SpikeReceiver* receiver){
    //assert(find(m_objects.begin(), m_objects.end(), emitter) != m_objects.end());
    //assert(find(m_objects.begin(), m_objects.end(), receiver) != m_objects.end());

    Axon* axon = new Axon(emitter, receiver);
    add_object(axon);
    receiver->add_incoming_axon(axon);
    return axon;
}

std::pair<Axon*, Synapse*> Network::connect(SpikeEmitter* emitter, DendriticNode* node){
    //assert(find(m_objects.begin(), m_objects.end(), emitter) != m_objects.end());
    //assert(find(m_objects.begin(), m_objects.end(), node) != m_objects.end());

    Synapse* synapse = new Synapse(node);
    add_object(synapse);
    Axon* axon = new Axon(emitter, synapse);
    add_object(axon);
    synapse->add_incoming_axon(axon);
    node->add_incoming_synapse(synapse);
    return std::pair<Axon*,Synapse*>(axon,synapse);
}
