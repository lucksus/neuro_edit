#ifndef DENDRITICNODE_H
#define DENDRITICNODE_H
#include <list>
#include "spatialobject.h"

class DendriticNode : public SpatialObject
{
public:
    DendriticNode(DendriticNode* parent);

    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();

    void add_current(double volt_per_second);
    double added_current();

    void reset();

private:
    double m_added_current;
    DendriticNode* m_parent;
    std::list<DendriticNode*> m_children;
};

#endif // DENDRITICNODE_H
