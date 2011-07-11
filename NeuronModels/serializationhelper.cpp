#include "serializationhelper.h"
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <fstream>
#include "neuron.h"
#include "izhikevich.h"
#include "axon.h"
#include "axonnode.h"
#include "synapse.h"
#include "dendriticnode.h"
#include "network.h"
#include "simulation.h"
#include "samples.h"
#include "lsmcolumn.h"

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

void SerializationHelper::clear_serialization_list(){
    m_serialization_list.clear();
}

SerializationHelper::SerializationHelper()
{
}

void SerializationHelper::serialize_simulation(std::ostream& stream, Simulation* simulation){
    boost::archive::xml_oarchive archive(stream);
    archive.register_type<Neuron>();
    archive.register_type<Izhikevich>();
    archive.register_type<Axon>();
    archive.register_type<AxonNode>();
    archive.register_type<Synapse>();
    archive.register_type<DendriticNode>();
    archive.register_type<Network>();
    archive.register_type<Simulation>();
    archive.register_type<Samples>();
    archive.register_type<CurrentInducer>();
    archive.register_type<LSMColumn>();

    try{
    archive << boost::serialization::make_nvp("simulation",*simulation);
    }
    catch(std::exception const& e) { std::cout << e.what() << std::endl;}
       catch(...) { std::cout << "whoops!" << std::endl; }
}

Simulation* SerializationHelper::deserialize_simulation(std::istream& stream){
    boost::archive::xml_iarchive archive(stream);
    archive.register_type<Neuron>();
    archive.register_type<Izhikevich>();
    archive.register_type<Axon>();
    archive.register_type<AxonNode>();
    archive.register_type<Synapse>();
    archive.register_type<DendriticNode>();
    archive.register_type<Network>();
    archive.register_type<Simulation>();
    archive.register_type<Samples>();
    archive.register_type<CurrentInducer>();
    archive.register_type<LSMColumn>();

    Simulation* s = new Simulation;
    try{
        archive >> boost::serialization::make_nvp("simulation",*s);
    }catch(std::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    catch(...) { std::cout << "whoops!" << std::endl; }

    return s;
}

void SerializationHelper::serialize_network(std::ostream& stream, Network* network){
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
    archive.register_type<Samples>();
    archive.register_type<CurrentInducer>();
    archive.register_type<LSMColumn>();

    try{
    archive << boost::serialization::make_nvp("network",*network);
    }
    catch(std::exception const& e) { std::cout << e.what() << std::endl;}
       catch(...) { std::cout << "whoops!" << std::endl; }
}

Network* SerializationHelper::deserialize_network(std::istream& stream){
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
    archive.register_type<Samples>();
    archive.register_type<CurrentInducer>();
    archive.register_type<LSMColumn>();

    Network* n = new Network;
    try{
        archive >> boost::serialization::make_nvp("network",*n);
    }catch(std::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    catch(...) { std::cout << "whoops!" << std::endl; }

    return n;
}

void SerializationHelper::serialize_objects(std::ostream& stream, std::set<SimulationObject*> objects){
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
    archive.register_type<Samples>();
    archive.register_type<CurrentInducer>();
    archive.register_type<LSMColumn>();

    try{
    archive << boost::serialization::make_nvp("objects",objects);
    }
    catch(std::exception const& e) { std::cout << e.what() << std::endl;}
       catch(...) { std::cout << "whoops!" << std::endl; }
}

std::set<SimulationObject*> SerializationHelper::deserialize_objects(std::istream& stream){
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
    archive.register_type<Samples>();
    archive.register_type<CurrentInducer>();
    archive.register_type<LSMColumn>();

    std::set<SimulationObject*> objects;
    try{
        archive >> boost::serialization::make_nvp("objects",objects);
    }catch(std::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    catch(...) { std::cout << "whoops!" << std::endl; }

    return objects;
}
