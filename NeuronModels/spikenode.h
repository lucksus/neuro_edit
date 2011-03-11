#ifndef SPIKENODE_H
#define SPIKENODE_H
#include "spikeemitter.h"
#include "spikereceiver.h"
#include "spatialobject.h"
#include <set>

class Axon;
class AxonNode : public SpikeEmitter, public SpikeReceiver, public SpatialObject
{
public:
    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();

    void add_receiver(Axon*);
    void remove_receiver(Axon*);

private:
    std::set<Axon*> m_receivers;
};

#endif // SPIKENODE_H
