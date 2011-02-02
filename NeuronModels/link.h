#ifndef LINK_H
#define LINK_H
#include "simulationobject.h"
#include <list>

class Neuron;
class Link : public SimulationObject
{
public:
    Link(Neuron* source, Neuron* destination, double weight=1, double speed=5);
    virtual void update(double milli_seconds);

private:
    Neuron *m_source, *m_destination;
    double m_weight;
    double m_speed; //mm per ms
    double m_runtime; //cached
    std::list<double> m_action_potentials;
};

#endif // LINK_H
