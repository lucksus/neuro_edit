#include "network.h"
#include <boost/foreach.hpp>
#include "synapse.h"
#include "axon.h"
#include <algorithm>
#include <assert.h>
#include "dendriticnode.h"
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>
#include <fstream>
#include "serializationhelper.h"
#include <exception>
#include "neuronmodel.h"
#include "izhikevich.h"
#include "spikenode.h"
#include <boost/serialization/type_info_implementation.hpp>
#include "neuron.h"

template<class Archive>
void register_classes(Archive& ar){

}

void Network::write_to_file(std::string filename){
    std::ofstream file(filename.c_str());
    serialize(file);
}

void Network::load_from_file(std::string filename){
    std::ifstream file(filename.c_str());
    deserialize(file);
}

void Network::serialize(std::ostream& stream){
    SerializationHelper::instance().set_serialize_all(true);
    boost::archive::xml_oarchive archive(stream);

    //archive.register_type<SimulationObject>();
    //archive.register_type<SpatialObject>();
    //archive.register_type<SpikeEmitter>();
    //archive.register_type<SpikeReceiver>();
    archive.register_type<Neuron>();
    //archive.register_type<NeuronModel>();
    archive.register_type<Izhikevich>();
    archive.register_type<Axon>();
    archive.register_type<AxonNode>();
    archive.register_type<Synapse>();
    archive.register_type<DendriticNode>();


    try{
    archive << boost::serialization::make_nvp("network",*this);
    }
    catch(std::exception const& e) { std::cout << e.what() << std::endl;}
       catch(...) { std::cout << "whoops!" << std::endl; }
}

void Network::deserialize(std::istream& stream){
    boost::archive::xml_iarchive archive(stream);

    //archive.register_type<SimulationObject>();
    //archive.register_type<SpatialObject>();
    //archive.register_type<SpikeEmitter>();
    //archive.register_type<SpikeReceiver>();
    archive.register_type<Neuron>();
    //archive.register_type<NeuronModel>();
    archive.register_type<Izhikevich>();
    archive.register_type<Axon>();
    archive.register_type<AxonNode>();
    archive.register_type<Synapse>();
    archive.register_type<DendriticNode>();

    try{
        archive >> boost::serialization::make_nvp("network",*this);
    }catch(std::exception const& e) {
        std::cout << e.what() << std::endl;
        //while (!stream.eof()) {
        //    std::string s;
        //    stream >> s;
        //    std::cout << " EXTRA CHARACTERS REMAINING: " << s << std::endl;
        //}
    }
    catch(...) { std::cout << "whoops!" << std::endl; }
}


void Network::add_object(SimulationObject* object){
    m_objects.push_back(object);
    emit object_added(object);
}

void Network::delete_object(SimulationObject* object){
    m_objects.remove(object);
    delete object;
    emit object_deleted(object);
}

std::list<SimulationObject*> Network::objects(){
    return m_objects;
}

void Network::simulate(double milli_seconds){
    BOOST_FOREACH(SimulationObject* o, m_objects){
        o->update(milli_seconds);
    }
}


Axon* Network::connect(SpikeEmitter* emitter, SpikeReceiver* receiver){
    //assert(find(m_objects.begin(), m_objects.end(), emitter) != m_objects.end());
    //assert(find(m_objects.begin(), m_objects.end(), receiver) != m_objects.end());

    Axon* axon = new Axon(emitter->neuron(), emitter, receiver);
    add_object(axon);
    receiver->add_incoming_axon(axon);
    return axon;
}

std::pair<Axon*, Synapse*> Network::connect(SpikeEmitter* emitter, DendriticNode* node){
    //assert(find(m_objects.begin(), m_objects.end(), emitter) != m_objects.end());
    //assert(find(m_objects.begin(), m_objects.end(), node) != m_objects.end());

    Synapse* synapse = new Synapse(emitter->neuron(), node);
    add_object(synapse);
    Axon* axon = new Axon(emitter->neuron(), emitter, synapse);
    add_object(axon);
    synapse->add_incoming_axon(axon);
    node->add_incoming_synapse(synapse);
    return std::pair<Axon*,Synapse*>(axon,synapse);
}
