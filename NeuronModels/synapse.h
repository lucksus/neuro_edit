#ifndef SYNAPSE_H
#define SYNAPSE_H
#include <list>
#include "spikereceiver.h"
#include "editableobject.h"

class DendriticNode;
class Synapse : public SpikeReceiver, public virtual EditableObject
{
public:
    Synapse(DendriticNode* target);

    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();

    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);

private:
    DendriticNode* m_postsynaptic_neuron;
    double m_weight;
    std::list<double> m_active_potentials;

};

#endif // SYNAPSE_H
