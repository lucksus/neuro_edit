#ifndef DRAWABLELSMCOLUMN_H
#define DRAWABLELSMCOLUMN_H
#include "drawable.h"
#include <QtCore/QRect>

class DrawableLSMColumn : public Drawable
{
public:
    DrawableLSMColumn(){dont_use_display_lists();}

    virtual bool is_applicable_to(SimulationObject*);
    virtual void set_color_and_lightning();
    virtual void draw_geometry_impl();

    QRect occupied_screen_region();

private:

};

#endif // DRAWABLELSMCOLUMN_H
