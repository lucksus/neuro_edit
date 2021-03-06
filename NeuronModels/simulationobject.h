#ifndef SIMULATIONOBJECT_H
#define SIMULATIONOBJECT_H
#include <set>
#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/basic_oarchive.hpp>
#include <QObject>
#include <QScriptEngine>
#include "point.h"
#include <boost/thread/recursive_mutex.hpp>

class Neuron;
class Network;
class Simulation;
class SimulationObject : public QObject
{
Q_OBJECT
//Q_PROPERTY(Point position READ position WRITE set_position)
Q_PROPERTY(Point* position READ pos WRITE set_pos)
friend class boost::serialization::access;
public:
    SimulationObject(Simulation*);
    SimulationObject(Neuron*);
    SimulationObject(const SimulationObject&);
    virtual ~SimulationObject(){}

    virtual void update(double milli_seconds) = 0;
    Q_INVOKABLE virtual std::set<SimulationObject*> children();
    Q_INVOKABLE virtual Neuron* neuron();
    Q_INVOKABLE Simulation* simulation();
    //virtual SimulationObject* clone() = 0;

    bool is_done();
    void reset_done();

    Q_INVOKABLE virtual void reset() {};

    Q_INVOKABLE void set_network(Network*);
    Q_INVOKABLE void set_neuron(Neuron*);

    Point position() const;
    void set_position(const Point& p);
    Point* pos();
    void set_pos(Point* p);


    virtual bool is_user_movable();
    void set_user_movable(bool user_movable);
    virtual Point moving_offset() const;

    //! Called when network wants to delete an object. Called before the object is deleted.
    /*!
     * Objects may return a list of objects that need to be delete too in consequence.
     */
    virtual std::set<SimulationObject*> about_to_remove(SimulationObject*);

    static QScriptValue toScriptValue(QScriptEngine *engine, SimulationObject* const &in)
    { return engine->newQObject(in); }

    static void fromScriptValue(const QScriptValue &object, SimulationObject* &out)
    { out = qobject_cast<SimulationObject*>(object.toQObject()); }

    void* bad_hacks[3];


    virtual std::list<std::string> user_actions() {return std::list<std::string>();}
    virtual std::set<std::string> active_user_actions();
    virtual void do_user_action(std::string) {}

protected:
    SimulationObject():m_neuron(0), m_simulation(0){}
    void done();
    virtual void moved(Point new_position);
    virtual void moved(Point new_position, Point old_position);

    void log(std::string);
    void debug(std::string);
    void error(std::string);

    Network* m_network;

    boost::recursive_mutex m_mutex;

private:
    Neuron* m_neuron;
    bool m_done;
    Point m_position;
    bool m_is_user_movable;
    Simulation* m_simulation;


    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(m_neuron);
        ar & boost::serialization::make_nvp("Position", m_position);
        ar & boost::serialization::make_nvp("is_user_movable", m_is_user_movable);
        ar & boost::serialization::make_nvp("Simulation", m_simulation);

        try{
            //this cast fails and throws std::bad_cast if ar is an iarchive.
            //so this block gets executed when saving,
            //the catch block when loading.
            (void) dynamic_cast<boost::archive::detail::basic_oarchive&>(ar);
            //----------
            //---SAVE:--
            //----------
            std::string name = objectName().toStdString();
            ar & boost::serialization::make_nvp("Name",name);
        }catch(std::bad_cast){
            //----------
            //---LOAD:--
            //----------
            std::string name;
            ar & boost::serialization::make_nvp("Name",name);
            setObjectName(name.c_str());
            bad_hacks[0] = bad_hacks[1] = bad_hacks[2] = 0;
        }
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(SimulationObject);
Q_DECLARE_METATYPE(SimulationObject*);
#endif // SIMULATIONOBJECT_H
