#ifndef NETWORK_H
#define NETWORK_H
#include <QtCore/QObject>
#include <list>
#include "simulationobject.h"

class Network : public QObject
{
Q_OBJECT
public:
    void add_object(SimulationObject*);
    void delete_object(SimulationObject*);
    std::list<SimulationObject*> objects();
    void simulate(double milli_seconds);

signals:
    void object_added(SimulationObject*);
    void object_deleted(SimulationObject*);
private:
    std::list<SimulationObject*> m_objects;
};

#endif // NETWORK_H
