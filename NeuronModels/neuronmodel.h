#ifndef NEURONMODEL_H
#define NEURONMODEL_H
#include "spikeemitter.h"
#include "editableobject.h"

class NeuronModel : public SpikeEmitter, public virtual EditableObject
{
public:
    virtual void update(double milli_seconds) = 0;
    virtual double membrane_potential() = 0;
    virtual void set_membrane_potential(double) = 0;

    void set_dendritic_current(double current);

protected:
    double dendritic_current();

private:
    double m_dendritic_current;
};

#endif // NEURONMODEL_H
