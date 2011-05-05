#include "scriptengine.h"
#include <QtGui/QAction>
#include "controller.h"
#include "simulation.h"
#include "network.h"

ScriptEngine::ScriptEngine()
{
    Simulation* sim = Controller::instance().simulation();
    if(!sim) return;
    QScriptValue objectValue = m_engine.newQObject(sim);
    m_engine.globalObject().setProperty("simulation", objectValue);
    Network* net = sim->network();
    if(!net) return;
    objectValue = m_engine.newQObject(net);
    m_engine.globalObject().setProperty("network", objectValue);
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
