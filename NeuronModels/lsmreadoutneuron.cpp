#include "lsmreadoutneuron.h"
#include <boost/foreach.hpp>
#include "spikingobject.h"
#include "RandomGenerator.h"
#include "group.h"
#include "neuron.h"

LSMReadOutNeuron::LSMReadOutNeuron()
 : m_time_constant(50), m_bias(1), m_bias_weight(1)
{}

LSMReadOutNeuron::LSMReadOutNeuron(Simulation* sim)
    : LinearDiscriminator(sim), m_time_constant(50), m_bias(1), m_bias_weight(1)
{
}

void LSMReadOutNeuron::init_weights_random_uniform(double min, double max){
    BOOST_FOREACH(ReadOutConnection& c, m_read_out_connections){
        c.weight = NeuroMath::RandomGenerator::getInstance()->uniform(min,max);
    }
    m_bias_weight = NeuroMath::RandomGenerator::getInstance()->uniform(min,max);
}

void LSMReadOutNeuron::update(double milli_seconds){
    LinearDiscriminator::update(milli_seconds);
    BOOST_FOREACH(ReadOutConnection& c, m_read_out_connections){
        if(c.liquid_unit->is_spiking()) c.active_potential = c.weight;
        else if(c.active_potential > 0) c.active_potential -= c.active_potential / 100 * m_time_constant * milli_seconds;
        else c.active_potential = 0;

        m_membrane_potential += c.active_potential;
    }

    m_membrane_potential += m_bias * m_bias_weight;

    BOOST_FOREACH(Samples* s, m_outputs){
        s->write_value(m_membrane_potential);
    }
}


void LSMReadOutNeuron::learn(double target, double rate){
    double d = (target - output()) * activation_function_derivative(m_membrane_potential);
    BOOST_FOREACH(ReadOutConnection& c, m_read_out_connections){
        c.weight += rate * d * c.active_potential;
    }
}

void LSMReadOutNeuron::connect_with_group(Group* group){
    m_read_out_connections.resize(group->children().size());
    BOOST_FOREACH(SimulationObject* o, group->objects_as_std_set()){
        ReadOutConnection r;
        SpikingObject* spo = dynamic_cast<SpikingObject*>(o);
        if(spo){
            r.liquid_unit = spo; r.active_potential = 0;
            m_read_out_connections.push_back(r);
        }

        Neuron* n = dynamic_cast<Neuron*>(o);
        if(n){
            r.liquid_unit = n->axon_root(); r.active_potential = 0;
            m_read_out_connections.push_back(r);
        }
    }
}


void LSMReadOutNeuron::set_bias(double b){
    m_bias = b;
}

double LSMReadOutNeuron::bias(){
    return m_bias;
}

void LSMReadOutNeuron::set_time_constant(double tc){
    m_time_constant = tc;
}

double LSMReadOutNeuron::time_constant(){
    return m_time_constant;
}


vector<SpikingObject*> LSMReadOutNeuron::liquid_units(){
    vector<SpikingObject*> ret_value;
    ret_value.resize(m_read_out_connections.size());
    unsigned int i=0;
    BOOST_FOREACH(ReadOutConnection& c, m_read_out_connections){
        ret_value[i] = c.liquid_unit;
        i++;
    }
    return ret_value;
}

const vector<ReadOutConnection>& LSMReadOutNeuron::read_out_connections(){
    return m_read_out_connections;
}

double LSMReadOutNeuron::bias_weight(){
    return m_bias_weight;
}
