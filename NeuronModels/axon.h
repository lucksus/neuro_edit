#ifndef LINK_H
#define LINK_H
#include "simulationobject.h"
#include <list>
#include "spikeemitter.h"
#include "spikereceiver.h"
#include "editableobject.h"

class Neuron;
class Axon : public SimulationObject, public EditableObject
{
public:
    Axon(SpikeEmitter* emitter, SpikeReceiver* receiver, double speed=5);
    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();

    double speed();
    void set_speed(double speed);
    std::list<double> action_potentials_normalized();

    void set_emitter(SpikeEmitter* emitter);

    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);

private:
    SpikeEmitter* m_emitter;
    SpikeReceiver* m_receiver;
    double m_speed; //mm per ms
    double m_runtime; //cached
    std::list<double> m_action_potentials;
};

#endif // LINK_H
