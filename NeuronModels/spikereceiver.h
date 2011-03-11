#ifndef SPIKERECEIVER_H
#define SPIKERECEIVER_H
#include "simulationobject.h"

class SpikeReceiver : virtual public SimulationObject
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
