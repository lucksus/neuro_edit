#include "neuron.h"
#include "dendriticnode.h"
#include "neuronmodel.h"
#include "axon.h"
#include "axonnode.h"
#include "izhikevich.h"
#include <boost/foreach.hpp>
#include <assert.h>
#include "network.h"

unsigned int Neuron::s_serial = 0;

Neuron::Neuron(){
    setObjectName(QString("Neuron_%1").arg(s_serial++));
}

Neuron::Neuron(Simulation* sim, Point position)
    : SimulationObject(sim), m_model(0)
{
    SimulationObject::set_neuron(this);
    setObjectName(QString("Neuron_%1").arg(s_serial++));
    m_dendrides_root = new DendriticNode(this, 0);
    m_axon_root = new AxonNode(this);
    set_position(position);
    set_model(new Izhikevich(this, 0.02, 0.2, -65, 8));
    m_axon_root->set_user_movable(false);
    m_dendrides_root->set_user_movable(false);
}


void Neuron::update(double milli_seconds){
    if(is_done()) return;
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

std::set<SimulationObject*> Neuron::children(){
    std::set<SimulationObject*> children;
    std::set<DendriticNode*> dendrites;
    walk_dendrites_tree(m_dendrides_root, dendrites);
    walk_axon_tree(m_axon_root, children);
    BOOST_FOREACH(DendriticNode* dn, dendrites){
        children.insert(dn);
    }
    return children;
}


void Neuron::walk_dendrites_tree(DendriticNode* root, std::set<DendriticNode*>& nodes){
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
        SpikingObject* receiver = axon->receiver();
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

std::set<SimulationObject*> Neuron::about_to_remove(SimulationObject* object_to_be_deleted){
    std::set<SimulationObject*> also_to_be_deleted = SimulationObject::about_to_remove(object_to_be_deleted);
    if(object_to_be_deleted == m_dendrides_root || object_to_be_deleted == m_axon_root)
        also_to_be_deleted.insert(this);
    return also_to_be_deleted;
}

AxonNode* Neuron::axon_root(){
    return m_axon_root;
}

DendriticNode* Neuron::dendrites_root(){
    return m_dendrides_root;
}

std::set<Synapse*> Neuron::incoming_synapses(){
    std::set<Synapse*> synapses;
    std::set<DendriticNode*> dendritic_nodes;
    walk_dendrites_tree(m_dendrides_root, dendritic_nodes);
    BOOST_FOREACH(DendriticNode* dn, dendritic_nodes){
        BOOST_FOREACH(Synapse* synapse, dn->incoming_synapses()){
            synapses.insert(synapse);
        }
    }
    return synapses;
}

void Neuron::reset(){
    m_model->reset();
}
