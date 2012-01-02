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
class Simulation;
class LinearDiscriminator;
class LDConnection;
class Network : public QObject
{
Q_OBJECT
friend class boost::serialization::access;
public:
    Network():m_script_engine(this){}
    Network(Simulation* sim):m_script_engine(this),m_simulation(sim){}
    Network(const Network& n):QObject(), m_objects(n.m_objects), m_script_engine(this), m_simulation(n.m_simulation){}

    void write_to_file(const std::string& filename);
    Q_INVOKABLE void write_to_file(const QString& filename);
    static Network* load_from_file(const std::string& filename);

    Q_INVOKABLE void add_object(SimulationObject*);
    Q_INVOKABLE void delete_object(SimulationObject*);
    void delete_objects(std::set<SimulationObject*>);
    std::set<SimulationObject*> objects_as_std_set();
    std::set<SimulationObject*> objects_with_children_as_std_set();
    Q_INVOKABLE QObjectList objects();
    Q_INVOKABLE SimulationObject* object_by_name(QString name);

    Q_INVOKABLE QStringList scripts() const;
    Q_INVOKABLE QString script(const QString&) const;
    Q_INVOKABLE void set_script(const QString& name, const QString& script);
    Q_INVOKABLE void run_script(const QString& name);
    Q_INVOKABLE void remove_script(const QString& name);
    Q_INVOKABLE Simulation* simulation(){return m_simulation;};

    Q_INVOKABLE Axon* connect(SpikingObject*, SpikingObject*);
    std::pair<Axon*, Synapse*> connect(SpikingObject*, DendriticNode*);
    LDConnection* connect(LinearDiscriminator* source, LinearDiscriminator* target);

    static QScriptValue networkToScriptValue(QScriptEngine *engine, Network* const &in)
    { return engine->newQObject(in); }

    static void networkFromScriptValue(const QScriptValue &object, Network* &out)
    { out = qobject_cast<Network*>(object.toQObject()); }

public:
    void simulate(double milli_seconds);

signals:
    void object_added(SimulationObject*);
    void object_deleted(SimulationObject*);
private:
    std::set<SimulationObject*> m_objects;
    std::map<std::string, std::string> m_scripts;
    ScriptEngine m_script_engine;
    Simulation* m_simulation;

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
