#ifndef LOG_H
#define LOG_H
#include <string>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

class SimulationObject;
class Simulation;

class Log
{
public:
    enum LogLevel{DEBUG_MSG=0, INFO_MSG=1, ERROR_MSG=2};

    static Log& instance();
    void log(string message, LogLevel log_level = INFO_MSG);
    void log(string message, SimulationObject* source, LogLevel log_level = INFO_MSG);
    void log(string message, Simulation* source, LogLevel log_level = INFO_MSG);

    const vector< pair<string, Log::LogLevel> > messages();

    void set_file(string filename);

private:
    Log();
    ~Log();
    ofstream m_default_file;
    map<Simulation*, ofstream*> m_files;
    vector< pair<string, LogLevel> > m_messages;
    ofstream* file_for(Simulation*);

    string time_stamp();
    string object_signature(SimulationObject*);
    void _log(string, Simulation*, LogLevel level);
};

#endif // LOG_H
