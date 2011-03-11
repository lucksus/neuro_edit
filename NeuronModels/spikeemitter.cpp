#include "spikeemitter.h"

void SpikeEmitter::reset_spike_emitter(){
    m_spiking = false;
}

bool SpikeEmitter::is_spiking(){
    return m_spiking;
}

void SpikeEmitter::emit_spike(){
    m_spikin = true;
}
