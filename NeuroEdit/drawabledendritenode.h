#ifndef DRAWABLEDENDRITENODE_H
#define DRAWABLEDENDRITENODE_H
#include "drawable.h"

class DrawableDendriteNode : public Drawable
{
public:
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry();

    enum {SIZE = 5};
};

#endif // DRAWABLEDENDRITENODE_H
