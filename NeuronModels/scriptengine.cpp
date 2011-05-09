#include "scriptengine.h"
#include <QtGui/QAction>
#include "controller.h"
#include "simulation.h"
#include "network.h"
#include "point.h"
#include "neuron.h"

ScriptEngine::ScriptEngine(Simulation* sim){
    if(!sim) return;
    //m_engine.setGlobalObject(m_engine.newQObject(sim));
    m_engine.globalObject().setProperty("simulation", m_engine.newQObject(sim));
    add_constructors();
}

ScriptEngine::ScriptEngine(Network* net){
    if(!net) return;
    //m_engine.setGlobalObject(m_engine.newQObject(net));
    m_engine.globalObject().setProperty("network", m_engine.newQObject(net));
    add_constructors();
}


QString ScriptEngine::evaluate(const QString& code){
    m_debugger.detach();
    QScriptValue return_value = m_engine.evaluate(code);
    return return_value.toString();
}

void ScriptEngine::debug(const QString& code){
    m_debugger.attachTo(&m_engine);
    m_debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();
    m_engine.evaluate(code);
}


QScriptValue Neuron_ctor(QScriptContext *ctx, QScriptEngine *eng)
 {
    Point p;
    if(ctx->argumentCount() == 3){
         p.x = ctx->argument(0).toNumber();
         p.y = ctx->argument(1).toNumber();
         p.z = ctx->argument(2).toNumber();
    }else if(ctx->argumentCount() == 1){
        QScriptValue point = ctx->argument(0);
        p.x = point.property("x").toNumber();
        p.y = point.property("y").toNumber();
        p.z = point.property("z").toNumber();
    }else assert(false);
    return eng->newQObject(new Neuron(p));
 }


void ScriptEngine::add_constructors(){
    m_engine.globalObject().setProperty("Neuron", m_engine.newFunction(Neuron_ctor));
}
