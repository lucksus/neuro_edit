#ifndef LSMREADOUTNEURON_H
#define LSMREADOUTNEURON_H
#include "lineardiscriminator.h"
#include <vector>
using namespace std;
class SpikingObject;
class Group;

struct ReadOutConnection{
    SpikingObject* liquid_unit;
    double weight, active_potential;
};


class LSMReadOutNeuron : public LinearDiscriminator
{
Q_OBJECT
Q_PROPERTY(double bias READ bias WRITE set_bias)
Q_PROPERTY(double synapse_time_constant READ time_constant WRITE set_time_constant)
public:
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
};

#endif // LSMREADOUTNEURON_H
