#include "neuronmodel.h"

NeuronModel::NeuronModel(Neuron* neuron)
    : SpikingObject(neuron)
{
}

void NeuronModel::set_dendritic_current(double current){
    m_dendritic_current = current;
}

double NeuronModel::dendritic_current(){
    return m_dendritic_current;
}
