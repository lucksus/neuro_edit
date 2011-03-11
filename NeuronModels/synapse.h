#ifndef SYNAPSE_H
#define SYNAPSE_H
#include <list>
#include "spikereceiver.h"

class DendriticNode;
class Synapse : public SpikeReceiver
{
public:
    Synapse(DendriticNode* target);

    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();

private:
    DendriticNode* m_postsynaptic_neuron;
    double m_weight;
    std::list<double> m_active_potentials;

};

#endif // SYNAPSE_H
