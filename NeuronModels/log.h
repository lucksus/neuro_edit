#ifndef LOG_H
#define LOG_H
#include <string>
#include <fstream>
#include <map>
#include <QtCore/QObject>

class SimulationObject;
class Simulation;

class Log : public QObject
{
Q_OBJECT
public:
    enum LogLevel{DEBUG, INFO, ERROR};

    static Log& instance();
    void log(std::string message, SimulationObject* source, LogLevel log_level = INFO);
    void log(std::string message, Simulation* source, LogLevel log_level = INFO);

signals:
    void new_log_message(QString text, Log::LogLevel level);

private:
    Log();
    ~Log();
    std::map<Simulation*, std::ofstream*> m_files;
    std::ofstream* file_for(Simulation*);

    std::string time_stamp();
    std::string object_signature(SimulationObject*);
    void _log(std::string, Simulation*);
};

#endif // LOG_H
