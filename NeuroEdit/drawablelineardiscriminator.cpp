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
#include "samples.h"

bool DrawableLinearDiscriminator::is_applicable_to(SimulationObject* object){
    LinearDiscriminator* n = dynamic_cast<LinearDiscriminator*>(object);
    return 0 != n;
}

void DrawableLinearDiscriminator::set_color_and_lightning(){
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DITHER);
    //GLfloat green[] = {.6,.6,.6,1};
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    LinearDiscriminator* n = dynamic_cast<LinearDiscriminator*>(m_object);
    double p = n->output();
    glColor4f(p,0.6,1-p,1.);

}

void DrawableLinearDiscriminator::draw_geometry_impl(){
    glPushMatrix();
    glRotated(90.,1.,0.,0.);
    GLUquadricObj* obj = gluNewQuadric();
    gluDisk(obj, 0, SIZE, 15, 1);
    gluDeleteQuadric(obj);
    glPopMatrix();

    LinearDiscriminator* ld = dynamic_cast<LinearDiscriminator*>(m_object);
    glColor3f(0.0,0.8,0.0);
    glLineWidth(1);
    glBegin(GL_LINES);
    BOOST_FOREACH(Samples* samples, ld->outputs()){
        glVertex3d(0, 0, 0);
        Point vec = samples->position() - ld->position();
        glVertex3d(vec.x, vec.y, vec.z);
    }
    glEnd();

}
