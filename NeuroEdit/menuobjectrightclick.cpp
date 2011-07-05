#include "menuobjectrightclick.h"
#include "simulationobject.h"
#include <boost/foreach.hpp>

MenuObjectRightClick::MenuObjectRightClick(SimulationObject* object)
    : m_object(object)
{
    BOOST_FOREACH(std::string action_text, m_object->user_actions()){
        QAction* action = addAction(action_text.c_str());
        connect(action, SIGNAL(triggered()), &m_signal_mapper, SLOT(map()));
        m_signal_mapper.setMapping(action, QString(action_text.c_str()));
    }
    connect(&m_signal_mapper, SIGNAL(mapped(QString)), this, SLOT(action_activated(QString)));
}

void MenuObjectRightClick::action_activated(QString action){
    m_object->do_user_action(action.toStdString());
}
