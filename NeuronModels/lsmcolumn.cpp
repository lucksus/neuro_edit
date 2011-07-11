#include "lsmcolumn.h"
#include "network.h"
#include "simulation.h"
#include <boost/foreach.hpp>
#include "RandomGenerator.h"
#include "math_constants.h"
#include <math.h>
#include <vector>

LSMColumn::LSMColumn(Simulation* sim)
    : SimulationObject(sim)
{
}



void LSMColumn::create_2d_grid(unsigned int width, unsigned int height, double distance){
    Point center = position();
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            Point neuron_pos = center;
            neuron_pos += Point(0,(y-static_cast<int>(height)/2)*distance,(x-static_cast<int>(width)/2)*distance);
            Neuron* neuron = new Neuron(neuron_pos);
            m_neurons.insert(neuron);
            simulation()->network()->add_object(neuron);
        }
    }
}

void LSMColumn::create_connections(double distance_constant){
    BOOST_FOREACH(Neuron* n1, m_neurons){
        BOOST_FOREACH(Neuron* n2, m_neurons){
            if(n1==n2) continue;
            double distance = n1->position().distance(n2->position());
            double rand = NeuroMath::RandomGenerator::getInstance()->uniform(0,1);
            if(rand < pow(NeuroMath::e(), -distance * distance_constant)){
                simulation()->network()->connect(n1->axon_root(), n2->dendrites_root());
            }
        }
    }
}

void LSMColumn::set_synapse_weights(double mean, double var){
    std::set<Synapse*> all_synapses;
    BOOST_FOREACH(Neuron* n, m_neurons){
        BOOST_FOREACH(Synapse* synapse, n->incoming_synapses()){
            all_synapses.insert(synapse);
        }
    }
    BOOST_FOREACH(Synapse* synapse, all_synapses){
        synapse->set_weight(NeuroMath::RandomGenerator::getInstance()->gauss(mean, var));
    }
}

void LSMColumn::connect_input_neuron_with_all(SpikingObject* input){
    BOOST_FOREACH(Neuron* n, m_neurons){
        simulation()->network()->connect(input, n->dendrites_root());
    }
}

void LSMColumn::connect_input_neuron_randomly(SpikingObject* input, unsigned int count){
    std::set<unsigned int> indices;
    std::vector<Neuron*> neurons(m_neurons.begin(),m_neurons.end());
    while(indices.size() < count){
        unsigned int rand = static_cast<unsigned int>(NeuroMath::RandomGenerator::getInstance()->uniform(1,indices.size()+1));
        indices.insert(rand);
    }
    BOOST_FOREACH(unsigned int index, indices){
        simulation()->network()->connect(input, neurons[index]->dendrites_root());
    }
}

std::set<Neuron*> LSMColumn::neurons(){
    return m_neurons;
}


std::list<std::string> LSMColumn::user_actions(){
    std::list<std::string> actions;
    actions.push_back("Create 2D grid...");
    actions.push_back("Create connections...");
    actions.push_back("Set synapse weights...");
    return actions;
}

void LSMColumn::do_user_action(std::string action){
    if("Create 2D grid..." == action){
        create_2d_grid(10,10,30);
    }
}

void LSMColumn::moved(Point new_position, Point old_position){
    Point vec = new_position - old_position;
    BOOST_FOREACH(Neuron* n, m_neurons){
        n->set_position(n->position() + vec);
    }
}
