#ifndef NEURON_H
#define NEURON_H
#include "spatialobject.h"

class Neuron : public SpatialObject
{
public:
    Neuron(Point position);
    Neuron(const Neuron& n);
    virtual ~Neuron() {};
    virtual bool update(double milli_seconds, double current) = 0;
    virtual double membrane_potential() = 0;
    virtual void set_membrane_potential(double) = 0;
    virtual bool is_spiking() = 0;

    void add_synaptic_input(double current);
    void update(double milli_seconds);
private:
    double m_summed_synaptic_inputs;
};

#endif // NEURON_H
