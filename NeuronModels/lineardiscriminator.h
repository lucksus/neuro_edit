#ifndef LINEARDISCRIMINATOR_H
#define LINEARDISCRIMINATOR_H
#include <set>
#include "simulationobject.h"
#include <boost/serialization/access.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/nvp.hpp>

class LDConnection;
class LinearDiscriminator : public SimulationObject
{
public:
    LinearDiscriminator();

    double output();

    virtual void update(double){};

    void add_input(LDConnection*);
    void remove_input(LDConnection*);

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("Inputs", m_inputs);
    }

private:
    std::set<LDConnection*> m_inputs;
};



namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, LinearDiscriminator>: public mpl::true_ {};
}

#endif // LINEARDISCRIMINATOR_H
