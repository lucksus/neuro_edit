#include "current_inducer.h"
#include "neuron.h"
#include <assert.h>

CurrentInducer::CurrentInducer(Simulation* s)
 : SimulationObject(s), m_target(0), m_current(0), m_active(false)
{
    set_user_movable(false);
}

CurrentInducer::CurrentInducer(DendriticNode* n) :
        SimulationObject(n->neuron()), m_target(n), m_current(0), m_active(false)
{
    set_user_movable(false);
    set_position(n->position());
    n->add_current_inducer(this);
}

SimulationObject* CurrentInducer::clone(){
    return new CurrentInducer(*this);
}

void CurrentInducer::set_current(double current){
    m_current = current;
}

double CurrentInducer::current(){
    return m_current;
}

void CurrentInducer::set_active(bool active){
    m_active = active;
}

bool CurrentInducer::active(){
    return m_active;
}

void CurrentInducer::update(double){
    if(m_active && m_target)
        m_target->add_current(m_current);
}
