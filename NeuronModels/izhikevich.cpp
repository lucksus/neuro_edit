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
    if(v >= 30){
        //spike
        v = c;
        u = u + d;
        m_spiking = false;
    }

    v += milli_seconds * (0.04*pow(v,2) + 5*v + 140 - u + current);
    u += milli_seconds * (a*(b*v - u));

    m_spiking = v >= 30;
    return m_spiking;
}

double Izhikevich::membrane_potential(){
    return v;
}

bool Izhikevich::is_spiking(){
    return m_spiking;
}
