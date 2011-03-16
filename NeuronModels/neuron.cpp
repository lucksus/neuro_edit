#include "neuron.h"
#include "dendriticnode.h"
#include "neuronmodel.h"
#include "axon.h"
#include "spikenode.h"
#include "izhikevich.h"
#include <boost/foreach.hpp>
#include <assert.h>

Neuron::Neuron(Point position)
    : SpatialObject(this), m_model(0), m_dendrides_root(this, 0), m_axon_root(this)
{
    set_position(position);
    set_model(new Izhikevich(this, 0.02, 0.2, -65, 8));
    m_axon_root.set_user_movable(false);
    m_dendrides_root.set_user_movable(false);
}

//Neuron::Neuron(const Neuron& n) :
//        EditableObject(n), SpatialObject(n), m_dendrides_root(this, 0)
//{
//}

void Neuron::update(double milli_seconds){
    m_dendrides_root.update(milli_seconds);
    m_model->set_dendritic_current(m_dendrides_root.added_current());
    m_model->update(milli_seconds);
    m_dendrides_root.reset();
    if(m_model->is_spiking()) m_axon_root.receive_spike();
    m_axon_root.update(milli_seconds);
}


void Neuron::set_model(NeuronModel* model){
    if(m_model) delete m_model;
    m_model = model;
}

double Neuron::membrane_potential(){
    return m_model->membrane_potential();
}


std::map<std::string, boost::any> Neuron::properties(){
    return m_model->properties();
}

void Neuron::set_property(std::string name, boost::any value){
    m_model->set_property(name, value);
}

std::set<SimulationObject*> Neuron::children(){
    std::set<SimulationObject*> nodes;
    walk_dendrites_tree(&m_dendrides_root, nodes);
    nodes.insert(&m_axon_root);
    return nodes;
}


void Neuron::walk_dendrites_tree(DendriticNode* root, std::set<SimulationObject*>& nodes){
    nodes.insert(root);
    BOOST_FOREACH(SimulationObject* node, root->children()){
        DendriticNode* n = dynamic_cast<DendriticNode*>(node);
        assert(n);
        walk_dendrites_tree(n, nodes);
    }
}


void Neuron::moved(Point new_position){
    Point p(-8,0,0);
    m_dendrides_root.set_position(new_position + p);
    m_axon_root.set_position(new_position - p);
}
