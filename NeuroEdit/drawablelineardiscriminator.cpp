#include "drawablelineardiscriminator.h"
#include "lineardiscriminator.h".h"
#ifdef WIN32
#include <Windows.h>
#endif
#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <assert.h>
#include "dendriticnode.h"
#include <boost/foreach.hpp>
#include "drawabledendritenode.h"
#include "glhelpfunctions.h"

bool DrawableLinearDiscriminator::is_applicable_to(SimulationObject* object){
    LinearDiscriminator* n = dynamic_cast<LinearDiscriminator*>(object);
    return 0 != n;
}

void DrawableLinearDiscriminator::set_color_and_lightning(){
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat green[] = {.6,.6,.6,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);

}

void DrawableLinearDiscriminator::draw_geometry_impl(){
    glRotated(90.,1.,0.,0.);
    GLUquadricObj* obj = gluNewQuadric();
    gluDisk(obj, 0, SIZE, 15, 1);
    gluDeleteQuadric(obj);

}
