#ifndef DRAWABLEAXON_H
#define DRAWABLEAXON_H
#include "drawable.h"
#include "point.h"

class SimulationObject;
class DrawableAxon : public Drawable
{
public:
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry();

    enum {SYNAPSE_SIZE = 5, SPIKE_SIZE = 7, AXON_RADIUS = 2};
};

#endif // DRAWABLEAXON_H
