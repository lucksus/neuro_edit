#include "simulation.h"
#include "network.h"
#include <sys/time.h>
#include <iostream>
#include <QtCore/QMutexLocker>

Simulation::Simulation():
    m_stop_request(false),
    m_network(0),
    m_simulation_step(.1),
    m_real_step(10)
{
}

void Simulation::request_stop(){
    m_stop_request = true;
}

void Simulation::wait_till_finished(){
    QMutexLocker locker(&m_mutex);
}

void Simulation::set_network(Network* network){
    m_network = network;
}

Network* Simulation::network(){
    return m_network;
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
    QMutexLocker locker(&m_mutex);
    m_time_ms = 0;
    emit simulation_started();
    while(!m_stop_request){
        struct timeval start, end;
        long real_time, seconds, useconds;
        gettimeofday(&start, NULL);
        m_network->simulate(m_simulation_step);
        m_time_ms += m_simulation_step;
        gettimeofday(&end, NULL);
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        real_time = seconds/1000000 + useconds;

        long diff = m_real_step*1000 - real_time;

        if(diff < 0) emit not_matching_speed();
        else usleep(diff);
        //emit simulation_milliseconds_passed(m_simulation_step);
    }
    m_stop_request = false;
    emit simulation_stopped();
}

double Simulation::time_ms(){
    return m_time_ms;
}

