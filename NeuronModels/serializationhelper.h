#ifndef SERIALIZATIONHELPER_H
#define SERIALIZATIONHELPER_H
#include <set>

class Neuron;
class SerializationHelper
{
public:
    static SerializationHelper& instance();
    bool is_to_be_serialized(Neuron*);
    void add_to_serialization_list(Neuron*);
    void set_serialize_all(bool);

private:
    SerializationHelper();
    std::set<Neuron*> m_serialization_list;
    bool m_serialize_all;
};

#endif // SERIALIZATIONHELPER_H
