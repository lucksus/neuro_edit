#ifndef NEURONMODEL_H
#define NEURONMODEL_H
#include "spikeemitter.h"

class NeuronModel : public SpikeEmitter
{
public:
    virtual void update(double milli_seconds, double current) = 0;
    virtual double membrane_potential() = 0;
    virtual void set_membrane_potential(double) = 0;
};

#endif // NEURONMODEL_H
