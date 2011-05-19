#ifndef DRAWABLEAXON_H
#define DRAWABLEAXON_H
#include "drawable.h"
#include "point.h"

class SimulationObject;
class DrawableAxon : public Drawable
{
public:
    DrawableAxon(){dont_use_display_lists();}
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();
    virtual bool do_translate_bevor_drawing() {return false;}

    enum {SYNAPSE_SIZE = 5, SPIKE_SIZE = 7, AXON_RADIUS = 2};
};

#endif // DRAWABLEAXON_H
