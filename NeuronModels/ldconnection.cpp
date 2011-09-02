#include "ldconnection.h"
#include "lineardiscriminator.h"

LDConnection::LDConnection(Simulation* sim)
    : SimulationObject(sim)
{
}


void LDConnection::set_weight(double weight){
    m_weight = weight;
}

double LDConnection::weight(){
    return m_weight;
}

void LDConnection::set_neurons(LinearDiscriminator* pre, LinearDiscriminator* post){
    m_pre = pre;
    m_post = post;
    post->add_input(this);
}

LinearDiscriminator* LDConnection::pre_neuron(){
    return m_pre;
}

LinearDiscriminator* LDConnection::post_neuron(){
    return m_post;
}
