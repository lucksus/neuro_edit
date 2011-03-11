#ifndef DENDRITICNODE_H
#define DENDRITICNODE_H

#include "spatialobject.h"

class DendriticNode : public SpatialObject
{
public:
    DendriticNode(DendriticNode* parent);

    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();

    void add_potential(double milli_volt);

    double added_potential();

private:
    double m_added_potential;
    DendriticNode* m_parent;
};

#endif // DENDRITICNODE_H
