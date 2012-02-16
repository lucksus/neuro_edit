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
    enum LogLevel{DEBUG=0, INFO=1, ERROR=2};

    static Log& instance();
    void log(string message, SimulationObject* source, LogLevel log_level = INFO);
    void log(string message, Simulation* source, LogLevel log_level = INFO);

    const vector< pair<string, Log::LogLevel> > messages();

private:
    Log();
    ~Log();
    map<Simulation*, ofstream*> m_files;
    vector< pair<string, LogLevel> > m_messages;
    ofstream* file_for(Simulation*);

    string time_stamp();
    string object_signature(SimulationObject*);
    void _log(string, Simulation*, LogLevel level);
};

#endif // LOG_H
