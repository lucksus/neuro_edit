#include "neuron.h"
#include "dendriticnode.h"
#include "neuronmodel.h"
#include "axon.h"

Neuron::Neuron(Point position)
    : m_model(0)
{
    set_position(position);
}

Neuron::Neuron(const Neuron& n) :
        SpatialObject(n),
        SpikeEmitter(n)
{
    m_summed_synaptic_inputs = n.m_summed_synaptic_inputs;
}

void Neuron::update(double milli_seconds){
    m_dendrides_root->update(milli_seconds);
    m_model->update(milli_seconds, m_dendrides_root->added_potential());
    m_dendrides_root->reset();
    m_axon_root->update(milli_seconds);
}


void Neuron::set_model(NeuronModel* model){
    if(m_model) delete m_model;
    m_model = model;
    m_axon_root->set_emitter(m_model);
}
