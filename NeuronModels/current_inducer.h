#ifndef CURRENT_INDUCER_H
#define CURRENT_INDUCER_H
#include "simulationobject.h"

class DendriticNode;
class CurrentInducer : public SimulationObject
{
public:
    CurrentInducer(Simulation*);
    CurrentInducer(DendriticNode*);
    ~CurrentInducer();

    virtual SimulationObject* clone();
    void set_current(double current);
    void set_active(bool);

    virtual void update(double milliseconds);

private:
    DendriticNode* m_target;
    double m_current;
    bool m_active;
};

#endif // CURRENT_INDUCER_H
