#ifndef NEURON_H
#define NEURON_H
#include "spatialobject.h"
#include "editableobject.h"
#include "dendriticnode.h"
#include "spikenode.h"
#include <boost/serialization/access.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/nvp.hpp>

class NeuronModel;
class Axon;
class Neuron : public SpatialObject, public virtual EditableObject
{
friend class boost::serialization::access;
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
    Neuron(){}
    virtual void moved(Point new_position);

private:

    NeuronModel* m_model;

    DendriticNode m_dendrides_root;
    AxonNode m_axon_root;

    void walk_dendrites_tree(DendriticNode* root, std::set<SimulationObject*>& nodes);


    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<SpatialObject>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpatialObject);
        ar & boost::serialization::make_nvp("NeuronModel", m_model);
        ar & boost::serialization::make_nvp("DendridesRoot", m_dendrides_root);
        ar & boost::serialization::make_nvp("AxonRoot", m_axon_root);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SpatialObject, Neuron>: public mpl::true_ {};
}

#endif // NEURON_H
