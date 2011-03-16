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
friend class boost::serialization::access;
public:
    void write_to_file(std::string filename);
    void load_from_file(std::string filename);

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

    void serialize(std::ostream&);
    void deserialize(std::istream&);

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("SimulationObjects", m_objects);
    }
};


#endif // NETWORK_H
