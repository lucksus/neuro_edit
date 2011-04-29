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
class SpikingObject;
class DendriticNode;
class Neuron;
class Network : public QObject
{
Q_OBJECT
friend class boost::serialization::access;
public:
    void write_to_file(std::string filename);
    static Network* load_from_file(std::string filename);

    void add_object(SimulationObject*);
    void delete_object(SimulationObject*);
    void delete_objects(std::set<SimulationObject*>);
    std::set<SimulationObject*> objects();


    Axon* connect(SpikingObject*, SpikingObject*);
    std::pair<Axon*, Synapse*> connect(SpikingObject*, DendriticNode*);

    void simulate(double milli_seconds);

signals:
    void object_added(SimulationObject*);
    void object_deleted(SimulationObject*);
private:
    std::set<SimulationObject*> m_objects;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("SimulationObjects", m_objects);
    }
};


#endif // NETWORK_H
