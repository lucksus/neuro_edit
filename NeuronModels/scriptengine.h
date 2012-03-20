#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H
#include <QtScript/QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>

class Simulation;
class Network;
class ScriptEngine
{
public:
    ScriptEngine();
    ScriptEngine(Simulation*);
    ScriptEngine(Network*);
    QString evaluate(const QString&);
    void debug(const QString&);

private:
    QScriptEngine m_engine;
    QScriptEngineDebugger m_debugger;

    void add_constructors();
    void add_global_functions();
    void add_conversion_functions();
    void add_application_objects();

};

#endif // SCRIPTENGINE_H
