#include "lineardiscriminator.h"
#include <boost/foreach.hpp>
#include "ldconnection.h"

LinearDiscriminator::LinearDiscriminator()
    : SimulationObject(static_cast<Neuron*>(0))
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
