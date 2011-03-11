#ifndef SPIKERECEIVER_H
#define SPIKERECEIVER_H
#include "spatialobject.h"

class SpikeReceiver : public virtual SpatialObject
{
public:
    void receive_spike();

protected:
    bool spike_received();
    void reset_received_spikes();

private:
    bool m_spike_received;

};

#endif // SPIKERECEIVER_H
