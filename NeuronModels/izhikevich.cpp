#include "izhikevich.h"
#include <math.h>

Izhikevich::Izhikevich(Neuron* neuron, double a, double b, double c, double d)
    : NeuronModel(neuron), m_u(0), m_v(c)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    setObjectName("Izhikevich Model");
}

Izhikevich::Izhikevich(const Izhikevich& i)
    : NeuronModel(i)
{
    m_v = i.m_v;
    m_u = i.m_u;
    a = i.a;
    b = i.b;
    c = i.c;
    d = i.d;
}

void Izhikevich::update(double milli_seconds){
    double current = dendritic_current();
    reset_spike_emitter();

    if(m_v >= 30){
        //spike
        m_v = c;
        m_u = m_u + d;
    }

    m_v += milli_seconds * (0.04*pow(m_v,2) + 5*m_v + 140 - m_u + current);
    m_u += milli_seconds * (a*(b*m_v - m_u));

    if(m_v >= 30) emit_spike();
}

double Izhikevich::membrane_potential(){
    return m_v;
}

void Izhikevich::set_membrane_potential(double p){
    m_v = p;
}


double Izhikevich::v(){
    return m_v;
}

double Izhikevich::u(){
    return m_u;
}

void Izhikevich::reset(){
    m_v = c;
    m_u = d;
}
