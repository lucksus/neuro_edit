#include "izhikevich.h"
#include <math.h>

Izhikevich::Izhikevich(double a, double b, double c, double d)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
}

Izhikevich::Izhikevich(const Izhikevich& i) :
{
    m_v = i.m_v;
    m_u = i.m_u;
    a = i.a;
    b = i.b;
    c = i.c;
    d = i.d;
}

SimulationObject* Izhikevich::clone(){
    return new Izhikevich(*this);
}

void Izhikevich::update(double milli_seconds, double current){
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
