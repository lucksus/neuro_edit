#include "izhikevich.h"
#include <math.h>

Izhikevich::Izhikevich(Neuron* neuron, double a, double b, double c, double d)
    : SpatialObject(neuron), NeuronModel(neuron)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
}

Izhikevich::Izhikevich(const Izhikevich& i)
    : EditableObject(i), SpatialObject(i), NeuronModel(i)
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


std::map<std::string, boost::any> Izhikevich::properties(){
    std::map<std::string, boost::any> result;
    result["a"] = a;
    result["b"] = b;
    result["c"] = c;
    result["d"] = d;
    return result;
}

void Izhikevich::set_property(std::string name, boost::any value){
    if("a" == name) a = boost::any_cast<double>(value);
    if("b" == name) b = boost::any_cast<double>(value);
    if("c" == name) c = boost::any_cast<double>(value);
    if("d" == name) d = boost::any_cast<double>(value);
}

