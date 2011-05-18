#ifndef DRAWABLENEURON_H
#define DRAWABLENEURON_H
#include "drawable.h"

class DrawableNeuron : public Drawable
{
public:
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();

    enum {SIZE = 15};
};

#endif // DRAWABLENEURON_H
