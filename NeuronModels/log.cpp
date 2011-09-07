#include "log.h"
#include <QtCore/QFileInfo>
#include "simulation.h"
#include <QtCore/QTime>
#include "neuron.h"
#include "group.h"
#include "samples.h"
#include <QtCore/QDir>
#include <boost/foreach.hpp>

Log::Log()
{
}

Log::~Log(){
    std::pair<Simulation*, std::ofstream*> it;
    BOOST_FOREACH(it, m_files){
        it.second->close();
        delete it.second;
    }
}

Log& Log::instance(){

}

void Log::log(std::string message, SimulationObject* source, LogLevel log_level){
    std::stringstream full_text;
    full_text << time_stamp() << " " << object_signature(source) << ": " << message;
    emit new_log_message(full_text.str(), log_level);
    std::stringstream with_log_level;
    if(log_level == DEBUG)
        with_log_level << "DEBUG: ";
    if(log_level == ERROR)
        with_log_level << "ERROR: ";
    with_log_level << full_text.str();

    _log(with_log_level.str(), source->simulation());
}

void Log::log(std::string message, Simulation* source, LogLevel log_level){
    std::stringstream full_text;
    full_text << time_stamp() << ": " << message;
    emit new_log_message(full_text.str(), log_level);
    std::stringstream with_log_level;
    if(log_level == DEBUG)
        with_log_level << "DEBUG: ";
    if(log_level == ERROR)
        with_log_level << "ERROR: ";
    with_log_level << full_text.str();

    _log(with_log_level.str(), source);
}

std::ofstream* Log::file_for(Simulation* sim){
    if(m_files.count(sim) <= 0){
        QFileInfo file_info(sim->filename().c_str());
        QString log_filename = file_info.absolutePath() + QDir::separator() + file_info.baseName() + "_log.txt";
        m_files[sim] = new std::ofstream(log_filename.toStdString().c_str());
    }

    return m_files[sim];
}


std::string Log::time_stamp(){
    return QTime::currentTime().toString("hh:mm:ss.zzz").toStdString();
}

std::string Log::object_signature(SimulationObject* object){
    std::stringstream ss;
    if(dynamic_cast<Neuron*>(object)) ss << "NEURON";
    if(dynamic_cast<Group*>(object)) ss << "GROUP";
    if(dynamic_cast<Samples*>(object)) ss << "SAMPLES";
    ss << "<" << object->objectName().toStdString() << ">";
    return ss.str();
}

void Log::_log(std::string text, Simulation* s){
    std::ofstream* file = file_for(s);
    (*file) << text << std::endl;
}
