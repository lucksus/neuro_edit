#ifndef MENUOBJECTRIGHTCLICK_H
#define MENUOBJECTRIGHTCLICK_H
#include <QtGui/QMenu>
#include <QtCore/QSignalMapper>
#include <QtCore/QSet>

class SimulationObject;
class MenuObjectRightClick : public QMenu
{
Q_OBJECT
public:
    MenuObjectRightClick(SimulationObject*);

public slots:
    void action_activated(QString action);

private:
    SimulationObject* m_object;
    QSignalMapper m_signal_mapper;
};

#endif // MENUOBJECTRIGHTCLICK_H