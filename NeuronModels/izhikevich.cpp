#include "izhikevich.h"
#include <math.h>

Izhikevich::Izhikevich(Point position, double a, double b, double c, double d)
    : Neuron(position)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    m_spiking = false;
}


bool Izhikevich::update(double milli_seconds, double current){
    if(m_v >= 30){
        //spike
        m_v = c;
        m_u = m_u + d;
        m_spiking = false;
    }

    m_v += milli_seconds * (0.04*pow(m_v,2) + 5*m_v + 140 - m_u + current);
    m_u += milli_seconds * (a*(b*m_v - m_u));

    m_spiking = m_v >= 30;
    return m_spiking;
}

double Izhikevich::membrane_potential(){
    return m_v;
}

void Izhikevich::set_membrane_potential(double p){
    m_v = p;
}

bool Izhikevich::is_spiking(){
    return m_spiking;
}


double Izhikevich::v(){
    return m_v;
}

double Izhikevich::u(){
    return m_u;
}
