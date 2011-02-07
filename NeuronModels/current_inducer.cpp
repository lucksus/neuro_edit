#include "current_inducer.h"
#include "neuron.h"

CurrentInducer::CurrentInducer(Neuron* n) :
        m_target(n), m_active(false)
{
}


void CurrentInducer::set_current(double current){
    m_current = current;
}

void CurrentInducer::set_active(bool active){
    m_active = active;
}

void CurrentInducer::update(double milliseconds){
    if(m_active)
        m_target->add_synaptic_input(m_current);
}