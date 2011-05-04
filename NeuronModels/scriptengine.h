#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H
#include <QtScript/QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>

class Network;
class Application;
class ScriptEngine
{
public:
    ScriptEngine(Network*);
    ScriptEngine(Application*);
    void evaluate(const QString&);
    void debug(const QString&);

private:
    QScriptEngine m_engine;
    QScriptEngineDebugger m_debugger;

};

#endif // SCRIPTENGINE_H
