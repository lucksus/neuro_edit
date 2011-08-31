#ifndef LINEARDISCRIMINATOR_H
#define LINEARDISCRIMINATOR_H
#include <set>
#include "simulationobject.h"
#include <boost/serialization/access.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/nvp.hpp>

class LDConnection;
class Simulation;
class LinearDiscriminator : public SimulationObject
{
friend class boost::serialization::access;
public:
    LinearDiscriminator(Simulation*);

    double output();

    virtual void update(double){};

    void add_input(LDConnection*);
    void remove_input(LDConnection*);

private:
    LinearDiscriminator(){}
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("Inputs", m_inputs);
    }

    std::set<LDConnection*> m_inputs;
};



namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, LinearDiscriminator>: public mpl::true_ {};
}

#endif // LINEARDISCRIMINATOR_H
