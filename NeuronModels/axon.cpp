#include "axon.h"
#include <boost/foreach.hpp>
#include "neuron.h"
#include <boost/thread/locks.hpp>

Axon::Axon(Neuron* neuron, SpikingObject* emitter, SpikingObject* receiver, double speed)
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
    boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
    m_runtime = m_emitter->position().distance(m_receiver->position()) / m_speed;
}

double Axon::speed(){
    boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
    return m_speed;
}

void Axon::set_speed(double speed){
    boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
    m_speed = speed;
    update_runtime();
}

bool leq_zero(double d){
    return d<=0;
}

void Axon::update(double milli_seconds){
    boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
    //add aps to list, if source is spiking
    if(m_emitter->is_spiking()){
        update_runtime();
        m_action_potentials.push_back(m_runtime);
    }

    if(is_done()) return;
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

    done();
    m_receiver->update(milli_seconds);
}

std::list<double> Axon::action_potentials_normalized(){
    boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
    std::list<double> result;
    BOOST_FOREACH(double time_to_go, m_action_potentials){
        result.push_back(1 - (time_to_go/m_runtime));
    }
    return result;
}


void Axon::set_emitter(SpikingObject* emitter){
    boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
    m_emitter = emitter;
}

SpikingObject* Axon::emitter(){
    boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
    return m_emitter;
}

SpikingObject* Axon::receiver(){
    boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
    return m_receiver;
}

std::set<SimulationObject*> Axon::about_to_remove(SimulationObject *object_to_be_deleted){
    std::set<SimulationObject*> also_to_be_deleted = SimulationObject::about_to_remove(object_to_be_deleted);
    if(m_receiver == object_to_be_deleted || m_emitter == object_to_be_deleted)
        also_to_be_deleted.insert(this);
    if(dynamic_cast<SpikingObject*>(object_to_be_deleted) == m_emitter)
        m_emitter = 0;
    if(dynamic_cast<SpikingObject*>(object_to_be_deleted) == m_receiver)
        m_receiver = 0;
    return also_to_be_deleted;
}

void Axon::reset(){
    m_action_potentials.clear();
}
