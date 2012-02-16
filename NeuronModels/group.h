#ifndef GROUP_H
#define GROUP_H
#include "simulationobject.h"
#include "qtstring_serialization.h"
#include <set>
#include <boost/serialization/set.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include "samples.h"

class Simulation;
class LinearDiscriminator;

class Group : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(QString user_action_script READ user_action_script WRITE set_user_action_script)
Q_PROPERTY(QString update_script READ update_script WRITE set_update_script)
Q_PROPERTY(double update_time_interval READ update_time_interval WRITE set_update_time_interval)
Q_PROPERTY(bool drawn_horizontally READ drawn_horizontally WRITE set_drawn_horizontally)
Q_PROPERTY(Samples* backprop_target READ backprop_target WRITE set_backprop_target)
Q_PROPERTY(LinearDiscriminator* mlp_output READ mlp_output WRITE set_mlp_output)
Q_PROPERTY(double eta READ backprop_eta WRITE set_backprop_eta)
friend class boost::serialization::access;
public:
    Group(Simulation*);

    virtual void update(double milli_seconds);
    Q_INVOKABLE virtual void reset();

    virtual std::list<std::string> user_actions();
    virtual std::set<std::string> active_user_actions();
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
    void set_backprop_target(Samples*);
    Samples* backprop_target();
    void set_mlp_output(LinearDiscriminator*);
    LinearDiscriminator* mlp_output();
    double backprop_eta();
    void set_backprop_eta(double);

    Q_INVOKABLE void add_input(Samples*);
    Q_INVOKABLE void remove_input(Samples*);
    std::set<Samples*> inputs();
    Q_INVOKABLE void add_object(SimulationObject*);
    Q_INVOKABLE void remove_object(SimulationObject*);
    Q_INVOKABLE virtual std::set<SimulationObject*> objects_as_std_set() const;
    Q_INVOKABLE virtual QObjectList objects() const;
    Q_INVOKABLE SimulationObject* object_by_name(QString name) const;

    enum {MARGIN=30};
    Point handle_position() const;
    virtual Point moving_offset() const;

    Q_INVOKABLE void create_mlp(unsigned int number_of_layers, std::vector<int> number_of_neurons);
    Q_INVOKABLE void create_2d_grid(unsigned int width, unsigned int height, double distance);
    Q_INVOKABLE void create_connections(double connection_scaling, double lambda);
    Q_INVOKABLE void set_synapse_weights(double mean, double var);

    Q_INVOKABLE void connect_input_neuron_with_all(SpikingObject*);
    Q_INVOKABLE void connect_input_neuron_randomly(SpikingObject*, unsigned int count);

    Q_INVOKABLE void do_backprop(double target_value, LinearDiscriminator* output_neuron);

    void backprop();

    std::set<Neuron*> neurons();
    std::set<LinearDiscriminator*> linear_discriminators();

protected:
    Group(){}
    virtual void moved(Point new_position, Point old_position);

private:
    QString m_user_actions_script;
    QString m_update_script;
    std::set<SimulationObject*> m_objects;
    std::set<Samples*> m_inputs;
    double m_update_time_interval;
    double m_elapsed_sim_time;
    bool m_drawn_horizontally;
    Samples* m_backprop_target;
    LinearDiscriminator* m_mlp_output;
    double m_backprop_eta;

    template<class Type1, class Type2>
    std::set<Type1*> extract_all(const std::set<Type2*>& s){
        std::set<Type1*> return_value;
        BOOST_FOREACH(Type2* o, s){
            Type1* n = dynamic_cast<Type1*>(o);
            if(n) return_value.insert(n);
        }
        return return_value;
    }

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
        ar & boost::serialization::make_nvp("BackpropTarget", m_backprop_target);
        ar & boost::serialization::make_nvp("MLPOutput", m_mlp_output);
        ar & boost::serialization::make_nvp("BackpropEta", m_backprop_eta);
    }

};


namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, Group>: public mpl::true_ {};
}


#endif // GROUP_H
