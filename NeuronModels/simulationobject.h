#ifndef SIMULATIONOBJECT_H
#define SIMULATIONOBJECT_H
#include <set>
#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/detail/basic_oarchive.hpp>
#include <QtCore/QObject>
#include <QtScript/QScriptEngine>
#include "point.h"

class Neuron;
class Network;
class Simulation;
class SimulationObject : public QObject
{
Q_OBJECT
Q_PROPERTY(Point position READ position WRITE set_position)
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

    void set_network(Network*);

    Point position() const;
    void set_position(const Point& p);

    virtual bool is_user_movable();
    void set_user_movable(bool user_movable);

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
    virtual void do_user_action(std::string) {}

protected:
    SimulationObject(){}
    void done();
    virtual void moved(Point new_position);
    virtual void moved(Point new_position, Point old_position);

    Network* m_network;

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
