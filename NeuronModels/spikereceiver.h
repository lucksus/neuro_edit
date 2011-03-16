#ifndef SPIKERECEIVER_H
#define SPIKERECEIVER_H
#include "spatialobject.h"

class Axon;
class SpikeReceiver : public virtual SpatialObject
{
public:
    SpikeReceiver(Neuron*);

    void receive_spike();

    void add_incoming_axon(Axon*);
    std::set<Axon*> incoming_axons();

protected:
    bool spike_received();
    void reset_received_spikes();

private:
    bool m_spike_received;
    std::set<Axon*> m_incoming_axons;

};

#endif // SPIKERECEIVER_H
