#ifndef SPIKENODE_H
#define SPIKENODE_H
#include "spikeemitter.h"
#include "spikereceiver.h"
#include "spatialobject.h"

class SpikeNode : public SpikeEmitter, public SpikeReceiver, public SpatialObject
{
public:
    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();
};

#endif // SPIKENODE_H
