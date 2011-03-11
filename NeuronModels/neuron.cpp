#include "neuron.h"
#include "dendriticnode.h"
#include "neuronmodel.h"
#include "axon.h"
#include "spikenode.h"
#include "izhikevich.h"

Neuron::Neuron(Point position)
    : m_model(0), m_dendrides_root(0)
{
    set_position(position);
    set_model(new Izhikevich(0.02, 0.2, -65, 8));
}

Neuron::Neuron(const Neuron& n) :
        EditableObject(n), SpatialObject(n), m_dendrides_root(0)
{
}

void Neuron::update(double milli_seconds){
    m_dendrides_root.update(milli_seconds);
    m_model->set_dendritic_current(m_dendrides_root.added_current());
    m_model->update(milli_seconds);
    m_dendrides_root.reset();
    if(m_model->is_spiking()) m_axon_root.receive_spike();
    m_axon_root.update(milli_seconds);
}


void Neuron::set_model(NeuronModel* model){
    if(m_model) delete m_model;
    m_model = model;
}

double Neuron::membrane_potential(){
    return m_model->membrane_potential();
}


std::map<std::string, boost::any> Neuron::properties(){
    return m_model->properties();
}

void Neuron::set_property(std::string name, boost::any value){
    m_model->set_property(name, value);
}
