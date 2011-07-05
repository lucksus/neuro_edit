#include "samples.h"
#include "simulation.h"
#include <stdexcept>
#include <boost/foreach.hpp>
#include "current_inducer.h"
#include "userinteractionadapter.h"
#include "photosssignalimporter.h"
#include <fstream>

Samples::Samples(Simulation* s)
    : SimulationObject(s), m_last_index(0)
{
}

void Samples::update(double){
    try{
        unsigned int i = find_current_index();

        double time = simulation()->time_ms();
        const sample& left = m_samples[i-1];
        const sample& right = m_samples[i];
        double rel_time = time - left.time;
        double fac = rel_time / (right.time - left.time);
        double value = left.value + fac*(right.value-left.value);

        BOOST_FOREACH(CurrentInducer* ci, m_current_inducers){
            ci->set_current(value);
        }

    }catch(std::out_of_range()){

    }
}

int Samples::find_current_index(){
    unsigned int i = m_last_index;
    double time = simulation()->time_ms();
    if(m_samples[i].time > time) throw std::out_of_range("no samples for current time value");
    while(m_samples[i].time < time && i < m_samples.size()) i++;
    if(i > m_samples.size() || i==0) throw std::out_of_range("no samples for current time value");

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

void Samples::set_samples(const std::vector<sample>& samples){
    m_samples = samples;
}

std::vector<sample> Samples::samples(){
    return m_samples;
}

void Samples::read_from_photoss_signal_file(std::string filename){

}


std::list<std::string> Samples::user_actions(){
    std::list<std::string> actions;
    actions.push_back("Load samples from PHOTOSS signal file...");
    actions.push_back("Plot samples");
    return actions;
}

void Samples::do_user_action(std::string action){
    if("Load samples from PHOTOSS signal file..." == action){
        std::string filename = UserInteractionAdapter::instance()->get_load_filepath("PHOTOSS signal file (*.txt)",objectName().toStdString(),"");
        if(filename.size() == 0) return;
        std::ifstream file;
        file.open(filename.c_str());
        PhotossSignalImporter importer(&file);
        m_samples = importer.read_samples(PhotossSignalImporter::ABSOLUTE_VALUES,1);
    }


    if("Plot samples" == action){
        UserInteractionAdapter::instance()->display_samples(m_samples);
    }
}
