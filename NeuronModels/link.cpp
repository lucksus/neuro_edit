#include "link.h"
#include <boost/foreach.hpp>
#include "neuron.h"

Link::Link(Neuron* source, Neuron* destination, double weight, double speed)
    : m_presynaptic_neuron(source), m_postsynaptic_neuron(destination), m_weight(weight), m_speed(speed)
{
    m_runtime = source->position().distance(destination->position()) / speed;
}

SimulationObject* Link::clone(){
    Link* link = new Link(m_presynaptic_neuron, m_postsynaptic_neuron, m_weight, m_speed);
    link->m_action_potentials = m_action_potentials;
    return link;
}

Neuron* Link::presynaptic_neuron(){
    return m_presynaptic_neuron;
}

Neuron* Link::postsynaptic_neuron(){
    return m_postsynaptic_neuron;
}

double Link::weight(){
    return m_weight;
}

double Link::speed(){
    return m_speed;
}

void Link::set_weight(double weight){
    m_weight = weight;
}

void Link::set_speed(double speed){
    m_speed = speed;
}

bool leq_zero(double d){
    return d<=0;
}

void Link::update(double milli_seconds){
    //propagating action potentials
    BOOST_FOREACH(double& d, m_action_potentials){
        d -= milli_seconds;
        if(d <= 0){
            //current ap has arrived at synapse
            m_postsynaptic_neuron->add_synaptic_input(m_weight);
        }
    }

    //remove arrived aps from list
    remove_if(m_action_potentials.begin(), m_action_potentials.end(), leq_zero);

    //add aps to list, if source is spiking
    if(m_presynaptic_neuron->is_spiking()){
        m_action_potentials.push_back(m_runtime);
    }
}
