#include "simulation.h"
#include "network.h"
#include <sys/time.h>

Simulation::Simulation()
    : m_stop_request(false), m_network(0)
{
}

void Simulation::request_stop(){
    m_stop_request = true;
}

void Simulation::set_network(Network* network){
    m_network = network;
}

void Simulation::set_time_step(double milliseconds){
    m_time_step = milliseconds;
}

void Simulation::set_time_ratio(double ratio){
    m_time_ratio = ratio;
}


void Simulation::run(){
    if(!m_network) return;
    emit simulation_started();
    while(!m_stop_request){
        struct timeval start, end;
        long real_time, seconds, useconds;
        gettimeofday(&start, NULL);
        m_network->simulate(m_time_step);
        gettimeofday(&end, NULL);
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        real_time = seconds/1000 + useconds;

        long sim_time = m_time_step*1000;
        long diff = sim_time*m_time_ratio - real_time;

        if(diff < 0) emit not_matching_speed();
        else usleep(diff);
    }
    m_stop_request = false;
    emit simulation_stopped();
}


