#ifndef NEURON_H
#define NEURON_H
#include "spatialobject.h"
#include "editableobject.h"
#include "dendriticnode.h"
#include "spikenode.h"

class NeuronModel;
class Axon;
class Neuron : public SpatialObject, public virtual EditableObject
{
public:
    Neuron(Point position);
    //Neuron(const Neuron& n);
    virtual ~Neuron() {};

    void update(double milli_seconds);

    void set_model(NeuronModel*);
    double membrane_potential();

    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);

    virtual std::set<SimulationObject*> children();

protected:
    virtual void moved(Point new_position);

private:

    NeuronModel* m_model;

    DendriticNode m_dendrides_root;
    AxonNode m_axon_root;

    void walk_dendrites_tree(DendriticNode* root, std::set<SimulationObject*>& nodes);
};

#endif // NEURON_H
