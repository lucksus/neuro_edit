#ifndef CURRENT_INDUCER_H
#define CURRENT_INDUCER_H
#include "simulationobject.h"

class DendriticNode;
class CurrentInducer : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(double current READ current WRITE set_current)
Q_PROPERTY(bool active READ active WRITE set_active)
public:
    CurrentInducer(Simulation*);
    CurrentInducer(DendriticNode*);

    virtual SimulationObject* clone();
    void set_current(double current);
    double current();
    void set_active(bool);
    bool active();

    virtual void update(double milliseconds);

private:
    DendriticNode* m_target;
    double m_current;
    bool m_active;
};

#endif // CURRENT_INDUCER_H
