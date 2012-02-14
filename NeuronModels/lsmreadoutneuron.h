#ifndef LSMREADOUTNEURON_H
#define LSMREADOUTNEURON_H
#include "lineardiscriminator.h"
#include <vector>
using namespace std;
class SpikingObject;
class Group;

struct ReadOutConnection{
    ReadOutConnection() : liquid_unit(0), weight(1), active_potential(0) {};

    SpikingObject* liquid_unit;
    double weight, active_potential;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("liquid_unit",liquid_unit);
        ar & boost::serialization::make_nvp("weight",weight);
        ar & boost::serialization::make_nvp("active_potential",active_potential);
    }
};


class LSMReadOutNeuron : public LinearDiscriminator
{
Q_OBJECT
Q_PROPERTY(double bias READ bias WRITE set_bias)
Q_PROPERTY(double synapse_time_constant READ time_constant WRITE set_time_constant)
friend class boost::serialization::access;
public:
    LSMReadOutNeuron();
    LSMReadOutNeuron(Simulation*);
    Q_INVOKABLE void init_weights_random_uniform(double min, double max);
    Q_INVOKABLE virtual void update(double milli_seconds);
    Q_INVOKABLE void learn(double target, double rate);
    Q_INVOKABLE void connect_with_group(Group*);
    Q_INVOKABLE void set_bias(double);
    Q_INVOKABLE double bias();
    Q_INVOKABLE void set_time_constant(double);
    Q_INVOKABLE double time_constant();

    vector<SpikingObject*> liquid_units();


private:
    vector<ReadOutConnection> m_read_out_connections;
    double m_time_constant;
    double m_bias, m_bias_weight;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LinearDiscriminator);
        ar & boost::serialization::make_nvp("read_out_connections",m_read_out_connections);
        ar & boost::serialization::make_nvp("time_constant",m_time_constant);
        ar & boost::serialization::make_nvp("bias",m_bias);
        ar & boost::serialization::make_nvp("bias_weight",m_bias_weight);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<LinearDiscriminator, LSMReadOutNeuron>: public mpl::true_ {};
}

#endif // LSMREADOUTNEURON_H
