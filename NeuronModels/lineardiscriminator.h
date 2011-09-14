#ifndef LINEARDISCRIMINATOR_H
#define LINEARDISCRIMINATOR_H
#include <set>
#include "simulationobject.h"
#include <boost/serialization/access.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/nvp.hpp>

class LDConnection;
class Simulation;
class DendriticNode;
class Samples;
class LinearDiscriminator : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(double output READ output)
Q_PROPERTY(double constant_output READ constant_output WRITE set_constant_output)
Q_PROPERTY(bool use_constant_output READ use_constant_output WRITE set_use_constant_output)
friend class boost::serialization::access;
public:
    LinearDiscriminator(){}
    LinearDiscriminator(Simulation*);

    virtual void update(double);

    void add_input(LDConnection*);
    void remove_input(LDConnection*);
    std::set<LDConnection*> inputs();

    void add_output(Samples*);
    void remove_output(Samples*);
    std::set<Samples*> outputs();

    double output();
    void set_constant_output(double);
    double constant_output();
    bool use_constant_output();
    void set_use_constant_output(bool);

    double activation_function(double);
    double activation_function_derivative(double);

    double membrane_potential();

    virtual std::list<std::string> user_actions();
    virtual void do_user_action(std::string);

    std::set<SimulationObject*> about_to_remove(SimulationObject* object_to_be_deleted);

protected:
    virtual void moved(Point new_position);

private:

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("Inputs", m_inputs);
        ar & boost::serialization::make_nvp("Outputs", m_outputs);
        ar & boost::serialization::make_nvp("MembranePotential", m_membrane_potential);
        ar & boost::serialization::make_nvp("ConstantInput", m_constant_output);
        ar & boost::serialization::make_nvp("DendriticNode", m_dendritic_node);
    }

    std::set<LDConnection*> m_inputs;
    std::set<Samples*> m_outputs;

    double m_membrane_potential;
    double m_constant_output;
    bool m_use_constant_output;
    DendriticNode* m_dendritic_node;
};



namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, LinearDiscriminator>: public mpl::true_ {};
}

Q_DECLARE_METATYPE(LinearDiscriminator)
Q_DECLARE_METATYPE(LinearDiscriminator*)

#endif // LINEARDISCRIMINATOR_H
