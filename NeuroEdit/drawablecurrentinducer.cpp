#include "drawablecurrentinducer.h"
#include "glhelpfunctions.h"
#include "current_inducer.h"

DrawableCurrentInducer::DrawableCurrentInducer()
{
}

bool DrawableCurrentInducer::is_applicable_to(SimulationObject* object){
    CurrentInducer* n = dynamic_cast<CurrentInducer*>(object);
    return 0 != n;
}

void DrawableCurrentInducer::set_color_and_lightning(){
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat color[] = {.8,8,.1,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

void DrawableCurrentInducer::draw_geometry_impl(){
    GLHelpFunctions::draw_frustum(Point(0,HEIGHT1,0),Point(0,HEIGHT2,-7),2,5,5,true,true);
}

