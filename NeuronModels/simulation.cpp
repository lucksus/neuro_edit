#include "simulation.h"
#include "network.h"
#include <sys/time.h>
#include <iostream>

Simulation::Simulation():
    m_stop_request(false),
    m_network(0),
    m_real_step(100),
    m_simulation_step(.01)

{
}

void Simulation::request_stop(){
    m_stop_request = true;
}

void Simulation::set_network(Network* network){
    m_network = network;
}

void Simulation::set_simulation_step(double milliseconds){
    m_simulation_step = milliseconds;
}

void Simulation::set_real_step(double milliseconds){
    m_real_step = milliseconds;
}

double Simulation::simulation_step(){
    return m_simulation_step;
}

double Simulation::real_step(){
    return m_real_step;
}


void Simulation::run(){
    if(!m_network) return;
    emit simulation_started();
    while(!m_stop_request){
        std::cout << "step" << std::endl;
        struct timeval start, end;
        long real_time, seconds, useconds;
        gettimeofday(&start, NULL);
        m_network->simulate(m_simulation_step);
        gettimeofday(&end, NULL);
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        real_time = seconds/1000000 + useconds;

        long diff = m_real_step*1000 - real_time;

        if(diff < 0) emit not_matching_speed();
        else usleep(diff);
        emit simulation_time_passed(m_simulation_step*1000);
    }
    m_stop_request = false;
    emit simulation_stopped();
}


