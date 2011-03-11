#include "neuronmodel.h"

void NeuronModel::set_dendritic_current(double current){
    m_dendritic_current = current;
}

double NeuronModel::dendritic_current(){
    return m_dendritic_current;
}
