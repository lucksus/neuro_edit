#ifndef SIMULATIONOBJECT_H
#define SIMULATIONOBJECT_H
#include <set>

class SimulationObject
{
public:
    virtual void update(double milli_seconds) = 0;
    virtual std::set<SimulationObject*> children();
    //virtual SimulationObject* clone() = 0;
};

#endif // SIMULATIONOBJECT_H
