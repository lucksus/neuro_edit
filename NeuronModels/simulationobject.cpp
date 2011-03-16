#include "simulationobject.h"


SimulationObject::SimulationObject(Neuron* neuron)
    : m_neuron(neuron)
{

}


std::set<SimulationObject*> SimulationObject::children(){
    return std::set<SimulationObject*>();
}


Neuron* SimulationObject::neuron(){
    return m_neuron;
}
