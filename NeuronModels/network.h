#ifndef NETWORK_H
#define NETWORK_H
#include <QtCore/QObject>
#include <list>
#include "simulationobject.h"
#include <boost/serialization/list.hpp>
#include <boost/serialization/nvp.hpp>
#include <ostream>
#include <istream>

class Axon;
class Synapse;
class SpikeEmitter;
class SpikeReceiver;
class DendriticNode;
class Network : public QObject
{
Q_OBJECT
public:
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(m_objects);
    }

    void serialize(std::ostream&);
    void deserialize(std::istream&);

    void add_object(SimulationObject*);
    void delete_object(SimulationObject*);
    std::list<SimulationObject*> objects();
    void simulate(double milli_seconds);

    Axon* connect(SpikeEmitter*, SpikeReceiver*);
    std::pair<Axon*, Synapse*> connect(SpikeEmitter*, DendriticNode*);

signals:
    void object_added(SimulationObject*);
    void object_deleted(SimulationObject*);
private:
    std::list<SimulationObject*> m_objects;
};


#endif // NETWORK_H
