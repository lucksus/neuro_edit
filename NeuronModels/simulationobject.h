#ifndef SIMULATIONOBJECT_H
#define SIMULATIONOBJECT_H

class SimulationObject
{
public:
    virtual void update(double milli_seconds) = 0;
};

#endif // SIMULATIONOBJECT_H
