#include "neuron.h"
#include "dendriticnode.h"
#include "neuronmodel.h"
#include "axon.h"
#include "axonnode.h"
#include "izhikevich.h"
#include <boost/foreach.hpp>
#include <assert.h>

Neuron::Neuron(Point position)
    : SpatialObject(this), m_model(0)
{
    m_dendrides_root = new DendriticNode(this, 0);
    m_axon_root = new AxonNode(this);
    set_position(position);
    set_model(new Izhikevich(this, 0.02, 0.2, -65, 8));
    m_axon_root->set_user_movable(false);
    m_dendrides_root->set_user_movable(false);
}

//Neuron::Neuron(const Neuron& n) :
//        EditableObject(n), SpatialObject(n), m_dendrides_root(this, 0)
//{
//}

void Neuron::update(double milli_seconds){
    m_dendrides_root->update(milli_seconds);
    m_model->set_dendritic_current(m_dendrides_root->added_current());
    m_model->update(milli_seconds);
    m_dendrides_root->reset();
    if(m_model->is_spiking()) m_axon_root->receive_spike();
    m_axon_root->update(milli_seconds);
    done();
}


void Neuron::set_model(NeuronModel* model){
    if(m_model) delete m_model;
    m_model = model;
}

NeuronModel* Neuron::model(){
    return m_model;
}

double Neuron::membrane_potential(){
    return m_model->membrane_potential();
}


Properties Neuron::properties(){
    Properties properties = m_model->properties();
    properties.merge(SpatialObject::properties());
    return properties;
}

void Neuron::set_property(std::string group, std::string name, boost::any value){
    SpatialObject::set_property(group,name,value);
    m_model->set_property(group, name, value);
}

std::set<SimulationObject*> Neuron::children(){
    std::set<SimulationObject*> nodes;
    walk_dendrites_tree(m_dendrides_root, nodes);
    walk_axon_tree(m_axon_root, nodes);
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

void Neuron::walk_axon_tree(AxonNode* root, std::set<SimulationObject*>& axon_objects){
    axon_objects.insert(root);
    BOOST_FOREACH(Axon* axon, root->receiving_axons()){
        axon_objects.insert(axon);
        SpikeReceiver* receiver = axon->receiver();
        AxonNode* node = dynamic_cast<AxonNode*>(receiver);
        Synapse* synapse = dynamic_cast<Synapse*>(receiver);
        if(synapse)
            axon_objects.insert(synapse);
        if(node)
            walk_axon_tree(node, axon_objects);
    }
}


void Neuron::moved(Point new_position){
    Point p(-8,0,0);
    m_dendrides_root->set_position(new_position + p);
    m_axon_root->set_position(new_position - p);
}
