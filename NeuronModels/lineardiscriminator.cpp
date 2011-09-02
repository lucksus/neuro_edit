#include "lineardiscriminator.h"
#include <boost/foreach.hpp>
#include "ldconnection.h"
#include "simulation.h"
#include "network.h"
#include "dendriticnode.h"

LinearDiscriminator::LinearDiscriminator(Simulation* s)
    : SimulationObject(s), m_membrane_potential(0.), m_constant_input(0.), m_dendritic_node(0)
{
}

void LinearDiscriminator::update(double d){
   m_membrane_potential = m_constant_input;
   BOOST_FOREACH(LDConnection* connection, m_inputs){
        connection->pre_neuron()->update(d);
        m_membrane_potential += connection->pre_neuron()->membrane_potential() * connection->weight();
   }
}

void LinearDiscriminator::add_input(LDConnection* connection){
    m_inputs.insert(connection);
}

void LinearDiscriminator::remove_input(LDConnection* connection){
    m_inputs.erase(connection);
}

std::set<LDConnection*> LinearDiscriminator::inputs(){
    return m_inputs;
}

double LinearDiscriminator::membrane_potential(){
    return m_membrane_potential;
}

void LinearDiscriminator::set_constant_input(double i){
    m_constant_input = i;
}

double LinearDiscriminator::constant_input(){
    return m_constant_input;
}

std::list<std::string> LinearDiscriminator::user_actions(){
    std::list<std::string> actions;
    actions.push_back("Update");
    actions.push_back("Create spiking dendrite node");
    return actions;
}

void LinearDiscriminator::do_user_action(std::string action){
    if("Update" == action){
        update(0.);
    }

    if("Create spiking dendrite node" == action){
        DendriticNode* node = new DendriticNode(this);
        node->set_user_movable(false);
        node->set_position(position());
        m_dendritic_node = node;
        simulation()->network()->add_object(node);
    }
}

void LinearDiscriminator::moved(Point new_position){
    if(m_dendritic_node)
        m_dendritic_node->set_position(new_position);
}

std::set<SimulationObject*> LinearDiscriminator::about_to_remove(SimulationObject* object_to_be_deleted){
    if(object_to_be_deleted == m_dendritic_node)
        m_dendritic_node = 0;
    return SimulationObject::about_to_remove(object_to_be_deleted);
}
