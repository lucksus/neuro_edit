#include "neuron.h"
#include "dendriticnode.h"
#include "neuronmodel.h"
#include "axon.h"
#include "izhikevich.h"

Neuron::Neuron(Point position)
    : m_model(0), m_dendrides_root(0), m_axon_root(0)
{
    set_position(position);
    set_model(new Izhikevich(0.02, 0.2, -65, 8));
    m_dendrides_root = new DendriticNode(0);
}

Neuron::Neuron(const Neuron& n) :
        SpatialObject(n)
{
}

void Neuron::update(double milli_seconds){
    m_dendrides_root->update(milli_seconds);
    m_model->set_dendritic_current(m_dendrides_root->added_current());
    m_model->update(milli_seconds);
    m_dendrides_root->reset();
    m_axon_root->update(milli_seconds);
}


void Neuron::set_model(NeuronModel* model){
    if(m_model) delete m_model;
    m_model = model;
    m_axon_root->set_emitter(m_model);
}
