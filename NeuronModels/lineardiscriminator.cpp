#include "lineardiscriminator.h"
#include <boost/foreach.hpp>
#include "ldconnection.h"
#include "simulation.h"
#include "network.h"
#include "dendriticnode.h"

LinearDiscriminator::LinearDiscriminator(Simulation* s)
    : SimulationObject(s), m_constant_input(0.), m_dendritic_node(0)
{
}

double LinearDiscriminator::output(){
   double sum = m_constant_input;
   BOOST_FOREACH(LDConnection* connection, m_inputs){
        sum += connection->pre_neuron()->output() * connection->weight();
   }
   return sum;
}

void LinearDiscriminator::add_input(LDConnection* connection){
    m_inputs.insert(connection);
}

void LinearDiscriminator::remove_input(LDConnection* connection){
    m_inputs.erase(connection);
}

void LinearDiscriminator::set_constant_input(double input){
    m_constant_input = input;
}

double LinearDiscriminator::constant_input(){
    return m_constant_input;
}

std::list<std::string> LinearDiscriminator::user_actions(){
    std::list<std::string> actions;
    actions.push_back("Create spiking dendrite node");
    return actions;
}

void LinearDiscriminator::do_user_action(std::string action){
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
