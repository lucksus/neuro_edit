#ifndef NEURON_H
#define NEURON_H
#include "spatialobject.h"

class DendriticNode;
class NeuronModel;
class Axon;
class SpikeNode;
class Neuron : public SpatialObject
{
public:
    Neuron(Point position);
    Neuron(const Neuron& n);
    virtual ~Neuron() {};

    void update(double milli_seconds);

    void set_model(NeuronModel*);
private:

    NeuronModel* m_model;

    DendriticNode* m_dendrides_root;
    Axon* m_axon_root;
};

#endif // NEURON_H
