#ifndef DRAWABLEAXONNODE_H
#define DRAWABLEAXONNODE_H
#include "drawable.h"

class SimulationObject;
class DrawableAxonNode : public Drawable
{
public:
    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();

    enum {SIZE = 5};
};

#endif // DRAWABLEAXONNODE_H
