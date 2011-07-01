#include "samples.h"
#include "simulation.h"
#include <stdexcept>
#include <boost/foreach.hpp>
#include "current_inducer.h"

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
