#ifndef SPIKEEMITTER_H
#define SPIKEEMITTER_H
#include "simulationobject.h"

class SpikeEmitter : virtual public SimulationObject
{
public:
    bool is_spiking();

protected:
    void emit_spike();
    void reset_spike_emitter();

private:
    bool m_spiking;
};

#endif // SPIKEEMITTER_H
