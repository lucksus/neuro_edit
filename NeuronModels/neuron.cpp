#include "neuron.h"

Neuron::Neuron(Point position)
    : m_summed_synaptic_inputs(0)
{
    set_position(position);
}

Neuron::Neuron(const Neuron& n) :
        SpatialObject(n)
{
    m_summed_synaptic_inputs = n.m_summed_synaptic_inputs;
}

void Neuron::add_synaptic_input(double current){
    m_summed_synaptic_inputs += current;
}

void Neuron::update(double milli_seconds){
    update(milli_seconds, m_summed_synaptic_inputs);
    m_summed_synaptic_inputs = 0;
}
