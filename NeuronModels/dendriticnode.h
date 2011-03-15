#ifndef DENDRITICNODE_H
#define DENDRITICNODE_H
#include <list>
#include "spatialobject.h"

class Synapse;
class DendriticNode : public SpatialObject
{
public:
    DendriticNode(DendriticNode* parent);

    virtual void update(double milli_seconds);
    virtual SimulationObject* clone();

    void add_current(double volt_per_second);
    double added_current();

    void reset();

    DendriticNode* parent();
    std::set<SimulationObject*> children();

    void add_incoming_synapse(Synapse*);
    void detach_incoming_synapse(Synapse*);

protected:
    virtual void moved(Point new_position);

private:
    double m_added_current;
    DendriticNode* m_parent;
    std::set<DendriticNode*> m_children;
    std::set<Synapse*> m_incoming_synapses;
};

#endif // DENDRITICNODE_H
