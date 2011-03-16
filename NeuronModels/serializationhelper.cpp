#include "serializationhelper.h"


SerializationHelper& SerializationHelper::instance(){
    static SerializationHelper sh;
    return sh;
}

bool SerializationHelper::is_to_be_serialized(Neuron* n){
    return m_serialize_all || (m_serialization_list.find(n) != m_serialization_list.end());
}

void SerializationHelper::add_to_serialization_list(Neuron* n){
    m_serialization_list.insert(n);
}

void SerializationHelper::set_serialize_all(bool b){
    m_serialize_all = b;
}

SerializationHelper::SerializationHelper()
{
}
