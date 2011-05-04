#include "scriptengine.h"
#include <QtGui/QAction>
#include "network.h"

ScriptEngine::ScriptEngine(Network* network)
{
    QScriptValue objectValue = m_engine.newQObject(network);
    m_engine.globalObject().setProperty("network", objectValue);
}

ScriptEngine::ScriptEngine(Application*){
}

void ScriptEngine::evaluate(const QString& code){
    m_debugger.detach();
    m_engine.evaluate(code);
}

void ScriptEngine::debug(const QString& code){
    m_debugger.attachTo(&m_engine);
    m_debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();
    m_engine.evaluate(code);
}
