#include "link.h"
#include <boost/foreach.hpp>
#include "neuron.h"

Link::Link(Neuron* source, Neuron* destination, double weight, double speed)
    : m_source(source), m_destination(destination), m_weight(weight), m_speed(speed)
{
    m_runtime = source->position().distance(destination->position()) / speed;
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
            m_destination->add_synaptic_input(m_weight);
        }
    }

    //remove arrived aps from list
    remove_if(m_action_potentials.begin(), m_action_potentials.end(), leq_zero);

    //add aps to list, if source is spiking
    if(m_source->is_spiking()){
        m_action_potentials.push_back(m_runtime);
    }
}
