#include "simulationobject.h"


SimulationObject::SimulationObject(Neuron* neuron)
    : m_neuron(neuron), m_done(false)
{

}


std::set<SimulationObject*> SimulationObject::children(){
    return std::set<SimulationObject*>();
}


Neuron* SimulationObject::neuron(){
    return m_neuron;
}

bool SimulationObject::is_done(){
    return m_done;
}

void SimulationObject::reset_done(){
    m_done = false;
}

void SimulationObject::done(){
    m_done = true;
}
