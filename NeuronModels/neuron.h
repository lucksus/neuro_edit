#ifndef NEURON_H
#define NEURON_H
#include "spatialobject.h"
#include "dendriticnode.h"
#include "axonnode.h"
#include <boost/serialization/access.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/nvp.hpp>

class NeuronModel;
class Axon;
class Neuron : public SpatialObject
{
Q_OBJECT
Q_PROPERTY(NeuronModel* model READ model WRITE set_model)
friend class boost::serialization::access;
public:
    Neuron();
    Neuron(Point position);
    //Neuron(const Neuron& n);
    void update(double milli_seconds);

    void set_model(NeuronModel*);
    NeuronModel* model();
    double membrane_potential();

    virtual std::set<SimulationObject*> children();

    std::set<SimulationObject*> about_to_remove(SimulationObject* object_to_be_deleted);

protected:
    virtual void moved(Point new_position);

private:

    NeuronModel* m_model;

    DendriticNode* m_dendrides_root;
    AxonNode* m_axon_root;

    void walk_dendrites_tree(DendriticNode* root, std::set<SimulationObject*>& nodes);
    void walk_axon_tree(AxonNode* root, std::set<SimulationObject*>& axon_objects);


    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<SpatialObject>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SpatialObject);
        ar & boost::serialization::make_nvp("NeuronModel", m_model);
        ar & boost::serialization::make_nvp("DendridesRoot", m_dendrides_root);
        ar & boost::serialization::make_nvp("AxonRoot", m_axon_root);
    }

    static unsigned int s_serial;
};

namespace boost{
template<>
struct is_virtual_base_of<SpatialObject, Neuron>: public mpl::true_ {};
}

Q_DECLARE_METATYPE(Neuron)
Q_DECLARE_METATYPE(Neuron*)
#endif // NEURON_H
