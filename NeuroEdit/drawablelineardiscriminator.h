#ifndef DRAWABLELINEARDISCRIMINATOR_H
#define DRAWABLELINEARDISCRIMINATOR_H
#include "drawable.h"

class DrawableLinearDiscriminator : public Drawable
{
public:
    DrawableLinearDiscriminator(){dont_use_display_lists();}
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();

    enum {SIZE = 15};
};

#endif // DRAWABLELINEARDISCRIMINATOR_H
