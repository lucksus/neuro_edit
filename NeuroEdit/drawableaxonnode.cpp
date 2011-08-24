#include "drawableaxonnode.h"
#include "axonnode.h"
#ifdef WIN32
#include <Windows.h>
#endif
#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

bool DrawableAxonNode::is_applicable_to(SimulationObject* object){
    AxonNode* n = dynamic_cast<AxonNode*>(object);
    return 0 != n;
}

void DrawableAxonNode::set_color_and_lightning(){
    GLfloat green[] = {.0,.9,.0,0.5};
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
}

void DrawableAxonNode::draw_geometry_impl(){
    GLUquadricObj* obj = gluNewQuadric();
    gluSphere(obj,SIZE,20,20);
    gluDeleteQuadric(obj);
}

