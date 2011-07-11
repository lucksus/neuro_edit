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

    virtual void update(double milli_seconds){};

    static LSMColumn* create_2d_column(unsigned int width, unsigned int height);


    void create_2d_grid(unsigned int width, unsigned int height, double distance);
    void create_connections(double distance_constant);
    void set_synapse_weights(double mean, double var);

    void connect_input_neuron_with_all(SpikingObject*);
    void connect_input_neuron_randomly(SpikingObject*, unsigned int count);

    std::set<Neuron*> neurons();

    virtual std::list<std::string> user_actions();
    virtual void do_user_action(std::string);

protected:
    virtual void moved(Point new_position, Point old_position);

private:
    LSMColumn(){}
    std::set<Neuron*> m_neurons;
};

#endif // LSMCOLUMN_H
