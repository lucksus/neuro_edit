#include "axon.h"
#include <boost/foreach.hpp>
#include "neuron.h"

Axon::Axon(Neuron* neuron, SpikeEmitter* emitter, SpikeReceiver* receiver, double speed)
    : SimulationObject(neuron), m_emitter(emitter), m_receiver(receiver), m_speed(speed)
{
    update_runtime();
    AxonNode* node = dynamic_cast<AxonNode*>(emitter);
    if(node)
        node->add_receiving_axon(this);
}

Axon::~Axon(){
    try{
    AxonNode* node = dynamic_cast<AxonNode*>(m_emitter);
    if(node)
        node->remove_receiving_axon(this);
    }catch(...){}
}

void Axon::update_runtime(){
    m_runtime = m_emitter->position().distance(m_receiver->position()) / m_speed;
}

double Axon::speed(){
    return m_speed;
}

void Axon::set_speed(double speed){
    m_speed = speed;
}

bool leq_zero(double d){
    return d<=0;
}

void Axon::update(double milli_seconds){
    //propagating action potentials
    BOOST_FOREACH(double& d, m_action_potentials){
        d -= milli_seconds;
        if(d <= 0){
            //current ap has arrived
            m_receiver->receive_spike();
        }
    }

    //remove arrived aps from list
    while((m_action_potentials.size() > 0) && (m_action_potentials.front()<=0))
        m_action_potentials.pop_front();

    //add aps to list, if source is spiking
    if(m_emitter->is_spiking()){
        update_runtime();
        m_action_potentials.push_back(m_runtime);
    }

    if(!m_receiver->is_done())
        m_receiver->update(milli_seconds);
    done();
}

std::list<double> Axon::action_potentials_normalized(){
    std::list<double> result;
    BOOST_FOREACH(double time_to_go, m_action_potentials){
        result.push_back(1 - (time_to_go/m_runtime));
    }
    return result;
}


void Axon::set_emitter(SpikeEmitter* emitter){
    m_emitter = emitter;
}

SpikeEmitter* Axon::emitter(){
    return m_emitter;
}

SpikeReceiver* Axon::receiver(){
    return m_receiver;
}


Properties Axon::properties(){
    Properties properties = EditableObject::properties();
    properties.set_group("Axon");
    properties.add("speed", m_speed);
    return properties;
}

void Axon::set_property(std::string group, std::string name, boost::any value){
    EditableObject::set_property(group, name, value);
    update_runtime();
    if("Axon" != group) return;
    if("speed" == name) m_speed = boost::any_cast<double>(value);
}


std::set<SimulationObject*> Axon::about_to_remove(SimulationObject *object_to_be_deleted){
    std::set<SimulationObject*> also_to_be_deleted = SimulationObject::about_to_remove(object_to_be_deleted);
    if(m_receiver == object_to_be_deleted || m_emitter == object_to_be_deleted)
        also_to_be_deleted.insert(this);
    if(dynamic_cast<SpikeEmitter*>(object_to_be_deleted) == m_emitter)
        m_emitter = 0;
    if(dynamic_cast<SpikeReceiver*>(object_to_be_deleted) == m_receiver)
        m_receiver = 0;
    return also_to_be_deleted;
}
