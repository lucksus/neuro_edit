#ifndef SPIKEEMITTER_H
#define SPIKEEMITTER_H
#include "spatialobject.h"

class SpikeEmitter : public virtual SpatialObject
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
