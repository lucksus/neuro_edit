#ifndef DRAWABLEDENDRITECONNECTION_H
#define DRAWABLEDENDRITECONNECTION_H
#include "drawable.h"

class DrawableDendriteConnection : public Drawable
{
public:
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();
};

#endif // DRAWABLEDENDRITECONNECTION_H
