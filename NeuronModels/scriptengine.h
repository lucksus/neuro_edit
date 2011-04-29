#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H
#include <QtScript/QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>
#include "network.h"

class ScriptEngine
{
public:
    ScriptEngine(Network*);
    void evaluate(const QString&);
    void debug(const QString&);

private:
    QScriptEngine m_engine;
    QScriptEngineDebugger m_debugger;

};

#endif // SCRIPTENGINE_H
