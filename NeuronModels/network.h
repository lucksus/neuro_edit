#ifndef NETWORK_H
#define NETWORK_H
#include <QtCore/QObject>
#include <list>
#include "simulationobject.h"
#include <boost/serialization/list.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <ostream>
#include <istream>
#include <map>
#include "scriptengine.h"
#include <QtCore/QStringList>
#include <QtCore/QObjectList>

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
    Network():m_script_engine(this){}
    Network(const Network& n):QObject(), m_objects(n.m_objects), m_script_engine(this){}

    void write_to_file(const std::string& filename);
    Q_INVOKABLE void write_to_file(const QString& filename);
    static Network* load_from_file(const std::string& filename);

    Q_INVOKABLE void add_object(SimulationObject*);
    Q_INVOKABLE void delete_object(SimulationObject*);
    void delete_objects(std::set<SimulationObject*>);
    std::set<SimulationObject*> objects_as_std_set();
    Q_INVOKABLE QObjectList objects();

    Q_INVOKABLE QStringList scripts() const;
    Q_INVOKABLE QString script(const QString&) const;
    Q_INVOKABLE void set_script(const QString& name, const QString& script);
    Q_INVOKABLE void run_script(const QString& name);
    Q_INVOKABLE void remove_script(const QString& name);

    Q_INVOKABLE Axon* connect(SpikingObject*, SpikingObject*);
    std::pair<Axon*, Synapse*> connect(SpikingObject*, DendriticNode*);

public:
    void simulate(double milli_seconds);

signals:
    void object_added(SimulationObject*);
    void object_deleted(SimulationObject*);
private:
    std::set<SimulationObject*> m_objects;
    std::map<std::string, std::string> m_scripts;
    ScriptEngine m_script_engine;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("SimulationObjects", m_objects);
        ar & boost::serialization::make_nvp("Scripts", m_scripts);
    }
};

Q_DECLARE_METATYPE(Network)
Q_DECLARE_METATYPE(Network*)

#endif // NETWORK_H
