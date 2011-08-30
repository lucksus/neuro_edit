#ifndef SERIALIZATIONHELPER_H
#define SERIALIZATIONHELPER_H
#include <set>
#include <ostream>

class Neuron;
class Network;
class Simulation;
class SimulationObject;
class SerializationHelper
{
public:
    static SerializationHelper& instance();
    bool is_to_be_serialized(Neuron*);
    void add_to_serialization_list(Neuron*);
    void set_serialize_all(bool);
    void clear_serialization_list();

    void serialize_simulation_xml(std::ostream&, Simulation*);
    Simulation* deserialize_simulation_xml(std::istream&);

    void serialize_simulation(std::ostream&, Simulation*);
    Simulation* deserialize_simulation(std::istream&);

    void serialize_network(std::ostream&, Network*);
    Network* deserialize_network(std::istream&);

    void serialize_objects(std::ostream&, std::set<SimulationObject*>);
    std::set<SimulationObject*> deserialize_objects(std::istream&);

private:
    SerializationHelper();
    std::set<Neuron*> m_serialization_list;
    bool m_serialize_all;
};

#endif // SERIALIZATIONHELPER_H
