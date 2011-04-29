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
#include <sstream>
#include "serializationhelper.h"
#include <exception>
#include "neuronmodel.h"
#include "izhikevich.h"
#include "axonnode.h"
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/foreach.hpp>
#include "neuron.h"

template<class Archive>
void register_classes(Archive& ar){

}

void Network::write_to_file(std::string filename){
    std::ofstream file(filename.c_str());
    SerializationHelper::instance().set_serialize_all(true);
    SerializationHelper::instance().serialize_network(file, this);
}

Network* Network::load_from_file(std::string filename){
    std::ifstream file(filename.c_str());
    Network* n = SerializationHelper::instance().deserialize_network(file);
    return n;
}

void Network::add_object(SimulationObject* object){
    m_objects.insert(object);
    object->set_network(this);
    emit object_added(object);
}

std::set<SimulationObject*> call_about_to_remove(std::set<SimulationObject*> objects, SimulationObject* object_to_be_deleted){
    std::set<SimulationObject*> also_to_be_removed;
    BOOST_FOREACH(SimulationObject* o, objects){
        std::set<SimulationObject*> s = call_about_to_remove(o->children(), object_to_be_deleted);
        BOOST_FOREACH(SimulationObject* oo, s){
            also_to_be_removed.insert(oo);
        }

        s = o->about_to_remove(object_to_be_deleted);

        BOOST_FOREACH(SimulationObject* oo, s){
            also_to_be_removed.insert(oo);
        }
    }
    return also_to_be_removed;
}

void Network::delete_object(SimulationObject* object_to_be_deleted){

    //Collect objects that can't live without object_to_be_deleted
    //to delete them too..
    std::set<SimulationObject*> also_to_be_removed, also_to_be_removed_old;
    also_to_be_removed = call_about_to_remove(m_objects, object_to_be_deleted);
    while(also_to_be_removed != also_to_be_removed_old){
        also_to_be_removed_old = also_to_be_removed;
        BOOST_FOREACH(SimulationObject* o, also_to_be_removed){
            std::set<SimulationObject*> s = call_about_to_remove(m_objects,o);
            BOOST_FOREACH(SimulationObject* oo, s){
                also_to_be_removed.insert(oo);
            }
        }
    }

    also_to_be_removed.insert(object_to_be_deleted);
    BOOST_FOREACH(SimulationObject* o, also_to_be_removed){
        m_objects.erase(o);
        delete o;
        emit object_deleted(o);
    }
}

void Network::delete_objects(std::set<SimulationObject*> objects){
    BOOST_FOREACH(SimulationObject* object_to_delete, objects){
        if(m_objects.count(object_to_delete)) delete_object(object_to_delete);
    }
}

std::set<SimulationObject*> Network::objects(){
    return m_objects;
}

void Network::simulate(double milli_seconds){
    BOOST_FOREACH(SimulationObject* o, m_objects){
        o->reset_done();
    }
    BOOST_FOREACH(SimulationObject* o, m_objects){
        if(!o->is_done())
            o->update(milli_seconds);
    }
}


Axon* Network::connect(SpikingObject* emitter, SpikingObject* receiver){
    //assert(find(m_objects.begin(), m_objects.end(), emitter) != m_objects.end());
    //assert(find(m_objects.begin(), m_objects.end(), receiver) != m_objects.end());

    Axon* axon = new Axon(emitter->neuron(), emitter, receiver);
    add_object(axon);
    receiver->add_incoming_axon(axon);
    return axon;
}

std::pair<Axon*, Synapse*> Network::connect(SpikingObject* emitter, DendriticNode* node){
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
