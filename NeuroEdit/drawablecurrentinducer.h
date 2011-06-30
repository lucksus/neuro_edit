#ifndef DRAWABLECURRENTINDUCER_H
#define DRAWABLECURRENTINDUCER_H
#include "drawable.h"

class DrawableCurrentInducer : public Drawable
{
public:
    DrawableCurrentInducer();
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();

    enum {HEIGHT1 = 2, HEIGHT2 = 20};
};

#endif // DRAWABLECURRENTINDUCER_H
