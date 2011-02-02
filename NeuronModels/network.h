#ifndef NETWORK_H
#define NETWORK_H
#include <list>
#include "simulationobject.h"

class Network
{
public:
    void add_object(SimulationObject*);
    void delete_object(SimulationObject*);
    std::list<SimulationObject*> objects();
    void simulate(double milli_seconds);

private:
    std::list<SimulationObject*> m_objects;
};

#endif // NETWORK_H
