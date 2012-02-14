#include "simulation.h"
#include "network.h"
#include <QtCore/QTime>
#include <iostream>
#include <QtCore/QMutexLocker>
#include <fstream>
#include "serializationhelper.h"
#include <boost/foreach.hpp>
#include <stdexcept>
#include "log.h"

Simulation::Simulation():
    m_stop_request(false),
    m_is_running(false),
    m_network(0),
    m_simulation_step(.1),
    m_real_step(10),
    m_time_ms(0),
    m_script_engine(this)
{
}

void Simulation::request_stop(){
    m_stop_request = true;
}

void Simulation::wait_till_finished(){
    QMutexLocker locker(&m_mutex);
}

void Simulation::write_to_file(const std::string& filename, Simulation::FileFormat format){
    std::ofstream file(filename.c_str());
    SerializationHelper::instance().set_serialize_all(true);
    switch(format){
        case XML: SerializationHelper::instance().serialize_simulation_xml(file, this);
            break;
        case BINARY: SerializationHelper::instance().serialize_simulation(file, this);
            break;
        default: assert(false);
    }
    Log::instance().log(QString("written to file %1").arg(filename.c_str()).toStdString(), this);
}

void Simulation::write_to_file(const QString& filename){
    FileFormat format = BINARY;
    if(filename.endsWith("nex")) format = XML;
    write_to_file(filename.toStdString(), format);
}

Simulation* Simulation::load_from_file(const std::string& filename, Simulation::FileFormat format){
    std::ifstream file(filename.c_str());
    Simulation* n=0;
    switch(format){
        case XML: n=SerializationHelper::instance().deserialize_simulation_xml(file);
            break;
        case BINARY: n=SerializationHelper::instance().deserialize_simulation(file);
            break;
        default: assert(false);
    }
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

void Simulation::set_filename(const std::string& filename){
    m_filename = filename;
}

std::string Simulation::filename(){
    return m_filename;
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
    Log::instance().log("Simulation started!", this);
    m_is_running = true;
    while(!m_stop_request){
		QTime time;
		time.start();
        m_network->simulate(m_simulation_step);
        m_time_ms += m_simulation_step;
		int real_time_ms = time.elapsed();

        long diff = m_real_step - real_time_ms;

        if(diff < 0) emit not_matching_speed();
        else msleep(diff);
        //emit simulation_milliseconds_passed(m_simulation_step);
    }
    m_is_running = false;
    m_stop_request = false;
    emit simulation_stopped();
    Log::instance().log("Simulation stoped!", this);
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

bool Simulation::is_running(){
    return m_is_running;
}

QString Simulation::evaluate_code(const QString& code){
    return m_script_engine.evaluate(code);
}
