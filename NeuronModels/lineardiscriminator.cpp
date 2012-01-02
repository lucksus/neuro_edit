#include "lineardiscriminator.h"
#include <boost/foreach.hpp>
#include "ldconnection.h"
#include "simulation.h"
#include "network.h"
#include "dendriticnode.h"
#include "samples.h"
#include "math_constants.h"

LinearDiscriminator::LinearDiscriminator(Simulation* s)
    : SimulationObject(s), m_membrane_potential(0.), m_constant_output(0.), m_dendritic_node(0), m_use_constant_output(false)
{
}

void LinearDiscriminator::update(double d){
   m_membrane_potential = 0;
   BOOST_FOREACH(LDConnection* connection, m_inputs){
        connection->pre_neuron()->update(d);
        m_membrane_potential += connection->pre_neuron()->output() * connection->weight();
   }
   BOOST_FOREACH(Samples* s, m_outputs){
       s->write_value(m_membrane_potential);
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

void LinearDiscriminator::add_output(Samples* s){
    m_outputs.insert(s);
}

void LinearDiscriminator::remove_output(Samples* s){
    m_outputs.erase(s);
}

std::set<Samples*> LinearDiscriminator::outputs(){
    return m_outputs;
}

double LinearDiscriminator::output(){
    if(m_use_constant_output) return m_constant_output;
    else return activation_function(m_membrane_potential);
}

void LinearDiscriminator::set_constant_output(double i){
    m_constant_output = i;
}

double LinearDiscriminator::constant_output(){
    return m_constant_output;
}

bool LinearDiscriminator::use_constant_output(){
    return m_use_constant_output;
}

void LinearDiscriminator::set_use_constant_output(bool b){
    m_use_constant_output = b;
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

double LinearDiscriminator::activation_function(double x){
    //return 1/(1+pow(NeuroMath::e(),-x))-0.5;
    return x;
}

double LinearDiscriminator::activation_function_derivative(double x){
    double x1 = activation_function(x+0.5);
    //return x1*(1-x1);
    return 1;
}

double LinearDiscriminator::membrane_potential(){
    return m_membrane_potential;
}
