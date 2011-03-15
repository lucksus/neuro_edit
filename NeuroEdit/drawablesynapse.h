#ifndef DRAWABLESYNAPSE_H
#define DRAWABLESYNAPSE_H
#include "drawable.h"

class DrawableSynapse : public Drawable
{
public:
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry();
};

#endif // DRAWABLESYNAPSE_H
