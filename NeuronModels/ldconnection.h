#ifndef LDCONNECTION_H
#define LDCONNECTION_H
#include "simulationobject.h"
#include <boost/serialization/access.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/nvp.hpp>

class LinearDiscriminator;
class LDConnection : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(double weight READ weight WRITE set_weight)
public:
    LDConnection(Simulation*);

    virtual void update(double){};

    void set_weight(double);
    double weight();

    void set_neurons(LinearDiscriminator* pre, LinearDiscriminator* post);
    LinearDiscriminator* pre_neuron();
    LinearDiscriminator* post_neuron();

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("PreNeuron", m_pre);
        ar & boost::serialization::make_nvp("PostNeuron", m_post);
        ar & boost::serialization::make_nvp("Weight", m_weight);
    }

private:
    LinearDiscriminator *m_pre, *m_post;
    double m_weight;
};



namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, LDConnection>: public mpl::true_ {};
}

//Q_DECLARE_METATYPE(LDConnection)
//Q_DECLARE_METATYPE(LDConnection*)

#endif // LDCONNECTION_H
