#include "lineardiscriminator.h"
#include <boost/foreach.hpp>
#include "ldconnection.h"
#include "simulation.h"

LinearDiscriminator::LinearDiscriminator(Simulation* s)
    : SimulationObject(s)
{
}

double LinearDiscriminator::output(){
    double sum=0;
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