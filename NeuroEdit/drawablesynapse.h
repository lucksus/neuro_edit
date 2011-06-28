#ifndef DRAWABLESYNAPSE_H
#define DRAWABLESYNAPSE_H
#include "drawable.h"
#include "point.h"

class Synapse;
class DrawableSynapse : public Drawable
{
public:
    DrawableSynapse() {dont_use_display_lists();}
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();

    enum {START_DISTANCE=10, END_DISTANCE=2, START_SIZE=1, END_SIZE=6};

    static Point displacement(Synapse*);
};

#endif // DRAWABLESYNAPSE_H
