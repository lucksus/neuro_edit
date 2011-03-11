#ifndef NEURON_H
#define NEURON_H
#include "spatialobject.h"
#include "editableobject.h"

class DendriticNode;
class NeuronModel;
class Axon;
class AxonNode;
class Neuron : public SpatialObject, public virtual EditableObject
{
public:
    Neuron(Point position);
    Neuron(const Neuron& n);
    virtual ~Neuron() {};

    void update(double milli_seconds);

    void set_model(NeuronModel*);


    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);
private:

    NeuronModel* m_model;

    DendriticNode* m_dendrides_root;
    Axon* m_axon_root;
};

#endif // NEURON_H
