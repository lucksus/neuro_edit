#ifndef SIMULATIONOBJECT_H
#define SIMULATIONOBJECT_H
#include <set>

class Neuron;
class SimulationObject
{
public:
    SimulationObject(Neuron*);
    virtual void update(double milli_seconds) = 0;
    virtual std::set<SimulationObject*> children();
    virtual Neuron* neuron();
    //virtual SimulationObject* clone() = 0;

private:
    Neuron* m_neuron;
};

#endif // SIMULATIONOBJECT_H
