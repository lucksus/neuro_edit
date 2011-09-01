#ifndef GROUP_H
#define GROUP_H
#include "simulationobject.h"
#include "qtstring_serialization.h"
#include <set>
#include <boost/serialization/set.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include "samples.h"

class Simulation;

class Group : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(QString user_action_script READ user_action_script WRITE set_user_action_script)
Q_PROPERTY(QString update_script READ update_script WRITE set_update_script)
Q_PROPERTY(double update_time_interval READ update_time_interval WRITE set_update_time_interval)
Q_PROPERTY(bool drawn_horizontally READ drawn_horizontally WRITE set_drawn_horizontally)
friend class boost::serialization::access;
public:
    Group(Simulation*);

    virtual void update(double milli_seconds);

    virtual std::list<std::string> user_actions();
    virtual void do_user_action(std::string);
    virtual std::set<SimulationObject*> about_to_remove(SimulationObject*);

    void set_user_action_script(QString);
    QString user_action_script();
    void set_update_script(QString);
    QString update_script();
    void set_update_time_interval(double);
    double update_time_interval();
    void set_drawn_horizontally(bool);
    bool drawn_horizontally() const;

    Q_INVOKABLE virtual void add_input(Samples*);
    Q_INVOKABLE virtual void remove_input(Samples*);
    Q_INVOKABLE virtual void add_object(SimulationObject*);
    Q_INVOKABLE virtual void remove_object(SimulationObject*);
    Q_INVOKABLE virtual std::set<SimulationObject*> objects_as_std_set() const;
    Q_INVOKABLE virtual QList<SimulationObject*> objects() const;

    enum {MARGIN=30};
    Point handle_position() const;
    virtual Point moving_offset() const;

protected:
    Group(){}

private:
    QString m_user_actions_script;
    QString m_update_script;
    std::set<SimulationObject*> m_objects;
    std::set<Samples*> m_inputs;
    double m_update_time_interval;
    double m_elapsed_sim_time;
    bool m_drawn_horizontally;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("UserActionsScript", m_user_actions_script);
        ar & boost::serialization::make_nvp("UpdateScript", m_update_script);
        ar & boost::serialization::make_nvp("Objects", m_objects);
        ar & boost::serialization::make_nvp("Inputs", m_inputs);
        ar & boost::serialization::make_nvp("UpdateTimeInterval", m_update_time_interval);
        ar & boost::serialization::make_nvp("ElapsedSimTime", m_elapsed_sim_time);
        ar & boost::serialization::make_nvp("DrawnHorizontally", m_drawn_horizontally);
    }

};


namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, Group>: public mpl::true_ {};
}


#endif // GROUP_H
