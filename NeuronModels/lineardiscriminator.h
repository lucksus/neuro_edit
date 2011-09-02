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
class LinearDiscriminator : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(double membrane_potential READ membrane_potential)
Q_PROPERTY(double constant_input READ constant_input WRITE set_constant_input)
friend class boost::serialization::access;
public:
    LinearDiscriminator(Simulation*);

    virtual void update(double);

    void add_input(LDConnection*);
    void remove_input(LDConnection*);

    double membrane_potential();
    void set_constant_input(double);
    double constant_input();

    virtual std::list<std::string> user_actions();
    virtual void do_user_action(std::string);

    std::set<SimulationObject*> about_to_remove(SimulationObject* object_to_be_deleted);

protected:
    virtual void moved(Point new_position);

private:
    LinearDiscriminator(){}
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("Inputs", m_inputs);
        ar & boost::serialization::make_nvp("MembranePotential", m_membrane_potential);
        ar & boost::serialization::make_nvp("ConstantInput", m_constant_input);
        ar & boost::serialization::make_nvp("DendriticNode", m_dendritic_node);
    }

    std::set<LDConnection*> m_inputs;

    double m_membrane_potential;
    double m_constant_input;
    DendriticNode* m_dendritic_node;
};



namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, LinearDiscriminator>: public mpl::true_ {};
}

#endif // LINEARDISCRIMINATOR_H
