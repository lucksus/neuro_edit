#ifndef LSMCOLUMN_H
#define LSMCOLUMN_H
#include "simulationobject.h"
#include "neuron.h"
#include <set>

class Simulation;
class LSMColumn : public SimulationObject
{
public:
    LSMColumn(Simulation*);

    static LSMColumn* create_2d_column(unsigned int width, unsigned int height);


    void create_2d_grid(unsigned int width, unsigned int height, double distance);
    void create_connections(double distance_constant);
    void set_synapse_weights(double mean, double sigma);

    void connect_input_neuron_with_all(SpikingObject*);
    void connect_input_neuron_randomly(SpikingObject*, unsigned int count);


private:
    LSMColumn(){}
    std::set<Neuron*> m_neurons;
};

#endif // LSMCOLUMN_H
