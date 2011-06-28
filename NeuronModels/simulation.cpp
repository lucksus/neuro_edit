#include "simulation.h"
#include "network.h"
#include <QtCore/QTime>
#include <iostream>
#include <QtCore/QMutexLocker>
#include <fstream>
#include "serializationhelper.h"
#include <boost/foreach.hpp>
#include <stdexcept>

Simulation::Simulation():
    m_stop_request(false),
    m_network(0),
    m_simulation_step(.1),
    m_real_step(10),
    m_script_engine(this)
{
}

void Simulation::request_stop(){
    m_stop_request = true;
}

void Simulation::wait_till_finished(){
    QMutexLocker locker(&m_mutex);
}

void Simulation::write_to_file(const std::string& filename){
    std::ofstream file(filename.c_str());
    SerializationHelper::instance().set_serialize_all(true);
    SerializationHelper::instance().serialize_simulation(file, this);
}

void Simulation::write_to_file(const QString& filename){
    write_to_file(filename.toStdString());
}

Simulation* Simulation::load_from_file(const std::string& filename){
    std::ifstream file(filename.c_str());
    Simulation* n = SerializationHelper::instance().deserialize_simulation(file);
    return n;
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
		QTime time;
		time.start();
        m_network->simulate(m_simulation_step);
        m_time_ms += m_simulation_step;
		int real_time_ms = time.elapsed();

        long diff = m_real_step - real_time_ms;

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


QStringList Simulation::scripts() const{
    QStringList list;
    std::pair<std::string, std::string> s;
    BOOST_FOREACH(s, m_scripts){
        list.append(s.first.c_str());
    }
    return list;
}

QString Simulation::script(const QString& name) const{
    if(m_scripts.count(name.toStdString()) < 1) throw std::logic_error(QString("Script %1 not found!").arg(name).toStdString());
    return QString(m_scripts.find(name.toStdString())->second.c_str());
}

void Simulation::set_script(const QString& name, const QString& script){
    m_scripts[name.toStdString()] = script.toStdString();
}


QString Simulation::run_script(const QString& name){
    return m_script_engine.evaluate(script(name));
}

void Simulation::remove_script(const QString& name){
    m_scripts.erase(name.toStdString());
}
