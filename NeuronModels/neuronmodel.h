#ifndef NEURONMODEL_H
#define NEURONMODEL_H
#include "spikingobject.h"
#include <boost/serialization/assume_abstract.hpp>

class NeuronModel : public SpikingObject
{
friend class boost::serialization::access;
public:
    NeuronModel(Neuron*);

    virtual void update(double milli_seconds) = 0;
    virtual double membrane_potential() = 0;
    virtual void set_membrane_potential(double) = 0;

    void set_dendritic_current(double current);

protected:
    NeuronModel(){}
    double dendritic_current();

private:
    double m_dendritic_current;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(NeuronModel);

Q_DECLARE_METATYPE(NeuronModel*)
#endif // NEURONMODEL_H
