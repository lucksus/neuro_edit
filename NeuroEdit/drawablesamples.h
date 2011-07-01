#ifndef DRAWABLESAMPLES_H
#define DRAWABLESAMPLES_H
#include "drawable.h"

class DrawableSamples : public Drawable
{
public:
    DrawableSamples();

    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();

    enum {WIDTH = 10, HEIGHT = 6, DEPTH = 6, SIZE = 20};
};

#endif // DRAWABLESAMPLES_H
