#include "scriptengine.h"
#include <QtGui/QAction>
#include "controller.h"
#include "simulation.h"
#include "network.h"
#include "point.h"
#include "neuron.h"
#include <QtCore/QMetaType>
#include "RandomGenerator.h"
#include "group.h"

ScriptEngine::ScriptEngine(Simulation* sim){
    if(!sim) return;
    m_engine.setGlobalObject(m_engine.newQObject(sim));
    add_constructors();
    add_global_functions();
    add_conversion_functions();
    add_random_generator();
}

ScriptEngine::ScriptEngine(Network* net){
    if(!net) return;
    m_engine.setGlobalObject(m_engine.newQObject(net));
    add_constructors();
    add_global_functions();
    add_conversion_functions();
    add_random_generator();
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
    Neuron* n = new Neuron(Controller::instance().simulation(), p);
    Controller::instance().simulation()->network()->add_object(n);
    return eng->newQObject(n);
 }

QScriptValue Group_ctor(QScriptContext* /*ctx*/, QScriptEngine *eng){
    Group* group_object = new Group(Controller::instance().simulation());
    Controller::instance().simulation()->network()->add_object(group_object);
    return eng->newQObject(group_object);
}

QScriptValue Point_ctor(QScriptContext *ctx, QScriptEngine *eng){
    Point *p = new Point();
    if(ctx->argumentCount() == 3){
         p->x = ctx->argument(0).toNumber();
         p->y = ctx->argument(1).toNumber();
         p->z = ctx->argument(2).toNumber();
    }else ctx->throwError("Point constructor needs three numbers!");
    return eng->newQObject(p, QScriptEngine::ScriptOwnership);
}

QScriptValue print(QScriptContext *ctx, QScriptEngine*)
{
    QString output;
    for(int i=0;i<ctx->argumentCount();i++){
        output += ctx->argument(i).toString();
    }
    Controller::instance().output_from_script(output);
    return QScriptValue();
}


void ScriptEngine::add_constructors(){
    m_engine.globalObject().setProperty("Neuron", m_engine.newFunction(Neuron_ctor));
    m_engine.globalObject().setProperty("Group", m_engine.newFunction(Group_ctor));
    m_engine.globalObject().setProperty("Point", m_engine.newFunction(Point_ctor));
}

void ScriptEngine::add_global_functions(){
    m_engine.globalObject().setProperty("print", m_engine.newFunction(print));
}

void ScriptEngine::add_conversion_functions(){
    qScriptRegisterMetaType(&m_engine, &Network::networkToScriptValue, &Network::networkFromScriptValue);
    qScriptRegisterMetaType(&m_engine, &SimulationObject::toScriptValue, &SimulationObject::fromScriptValue);
    qScriptRegisterMetaType(&m_engine, &Point::toScriptValue, &Point::fromScriptValue);
}

void ScriptEngine::add_random_generator(){
    m_engine.globalObject().setProperty("RandomGenerator", m_engine.newQObject(NeuroMath::RandomGenerator::getInstance()));
}
