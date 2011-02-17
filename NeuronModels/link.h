#ifndef LINK_H
#define LINK_H
#include "simulationobject.h"
#include <list>

class Neuron;
class Link : public SimulationObject
{
public:
    Link(Neuron* presynaptic_neuron, Neuron* postsynaptic_neuron, double weight=1, double speed=5);
    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();

    Neuron* presynaptic_neuron();
    Neuron* postsynaptic_neuron();
    double weight();
    double speed();
    void set_weight(double weight);
    void set_speed(double speed);

private:
    Neuron *m_presynaptic_neuron, *m_postsynaptic_neuron;
    double m_weight;
    double m_speed; //mm per ms
    double m_runtime; //cached
    std::list<double> m_action_potentials;
};

#endif // LINK_H
