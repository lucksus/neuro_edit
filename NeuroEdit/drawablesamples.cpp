#include "drawablesamples.h"
#include "samples.h"
#include "glhelpfunctions.h"
#include <GLUT/glut.h>
DrawableSamples::DrawableSamples()
{
    dont_use_display_lists();
}


bool DrawableSamples::is_applicable_to(SimulationObject* object){
    Samples* n = dynamic_cast<Samples*>(object);
    return 0 != n;
}

void DrawableSamples::set_color_and_lightning(){
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat color[] = {.75,0.,0.,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

void DrawableSamples::draw_geometry_impl(){
    glutSolidCube(SIZE);
}

