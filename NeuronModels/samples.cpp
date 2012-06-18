#include "samples.h"
#include "simulation.h"
#include <stdexcept>
#include <boost/foreach.hpp>
#include "current_inducer.h"
#include "userinteractionadapter.h"
#include "photosssignalimporter.h"
#include <fstream>
#include "serializationhelper.h"

unsigned int Samples::s_serial = 0;

Samples::Samples(Simulation* s)
    : SimulationObject(s), m_last_index(0), m_last_time(0), m_current_value(0), m_constant_value_active(false), m_constant_value(0)
{
    setObjectName(QString("Samples_%1").arg(s_serial++));
}

void Samples::update(double time){
    if(!m_constant_value_active && m_samples.size() == 0) return;

    static double last_debug=0;
    double value;
    if(m_constant_value_active)
        value = m_constant_value;
    else{
        double time = simulation()->time_ms();
        if(time < m_last_time)
            m_last_index=0;
        m_last_time = time;

        int i = find_current_index();
        if(i<0 || i>=m_samples.size()){
            if(last_debug > 100){
                debug("out of range!");
                last_debug = 0;
            }
            value = 0;
            return;
        }

        const sample& left = m_samples[i-1];
        const sample& right = m_samples[i];
        double rel_time = time - left.time;
        double fac = rel_time / (right.time - left.time);
        value = left.value + fac*(right.value-left.value);
    }

    BOOST_FOREACH(CurrentInducer* ci, m_current_inducers){
        ci->set_current(value);
    }

    BOOST_FOREACH(LinearDiscriminator* ld, m_linear_discriminators){
        ld->set_constant_output(value);
    }

    m_current_value = value;

    last_debug += time;

}

int Samples::find_current_index(){
    unsigned int i = m_last_index;
    double time = simulation()->time_ms();
    if(i==0 && m_samples[i].time > time) return -1;
    while(i < m_samples.size() && m_samples[i].time < time) i++;
    if(i > m_samples.size() || i==0) return -1;

    m_last_index = i;
    return i;
}


void Samples::add_current_inducer(CurrentInducer* ci){
    m_current_inducers.push_back(ci);
}

void Samples::remove_current_inducer(CurrentInducer* ci){
    m_current_inducers.remove(ci);
}

const std::list<CurrentInducer*>& Samples::current_inducers(){
    return m_current_inducers;
}

void Samples::add_linear_discriminator(LinearDiscriminator* ld){
    m_linear_discriminators.insert(ld);
}

void Samples::remove_linear_discriminator(LinearDiscriminator* ld){
    m_linear_discriminators.erase(ld);
}

const std::set<LinearDiscriminator*>& Samples::linear_discriminators(){
    return m_linear_discriminators;
}

void Samples::set_samples(const std::vector<sample>& samples){
    m_samples = samples;
}

std::vector<sample> Samples::samples(){
    return m_samples;
}

void Samples::read_from_photoss_signal_file(std::string filename){
    std::ifstream file;
    file.open(filename.c_str());
    PhotossSignalImporter importer(&file);
    m_samples = importer.read_samples(PhotossSignalImporter::ABSOLUTE_VALUES,1);
}


std::list<std::string> Samples::user_actions(){
    std::list<std::string> actions;
    actions.push_back("Save samples...");
    actions.push_back("Load samples...");
    actions.push_back("Load samples from PHOTOSS signal file...");
    actions.push_back("Plot samples");
    actions.push_back("Clear");
    return actions;
}

void Samples::do_user_action(std::string action){
    if("Save samples..." == action){
        std::string filename = UserInteractionAdapter::instance()->get_save_filepath("Samples file (*.nsamples)",objectName().toStdString(),"");
        if(filename.size() == 0) return;
        save_samples_to_file(filename);
    }

    if("Load samples..." == action){
        std::string filename = UserInteractionAdapter::instance()->get_load_filepath("Samples file (*.nsamples)",objectName().toStdString(),"");
        if(filename.size() == 0) return;
        load_samples_from_file(filename);
    }

    if("Load samples from PHOTOSS signal file..." == action){
        std::string filename = UserInteractionAdapter::instance()->get_load_filepath("PHOTOSS signal file (*.txt)",objectName().toStdString(),"");
        if(filename.size() == 0) return;
        read_from_photoss_signal_file(filename);
    }


    if("Plot samples" == action){
        UserInteractionAdapter::instance()->display_samples(m_samples, objectName().toStdString());
    }

    if("Clear" == action){
        clear();
    }
}

void Samples::write_value(double value){
    sample s;
    s.value = value;
    s.time = simulation()->time_ms();
    m_samples.push_back(s);
}

double Samples::value(){
    return m_current_value;
}

void Samples::set_constant_value_active(bool b){
    m_constant_value_active = b;
}

bool Samples::constant_value_active(){
    return m_constant_value_active;
}

void Samples::set_constant_value(double d){
    m_constant_value = d;
}

double Samples::constant_value(){
    return m_constant_value;
}

void Samples::clear(){
    m_samples.clear();
    m_last_index = 0;
    m_last_time = 0;
}

void Samples::save_samples_to_file(std::string filename){
    std::ofstream file(filename.c_str());
    SerializationHelper::instance().serialize_samples(file, m_samples);
}

void Samples::load_samples_from_file(std::string filename){
    std::ifstream file(filename.c_str());
    m_samples = SerializationHelper::instance().deserialize_samples(file);
}
