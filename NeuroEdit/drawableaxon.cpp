#include "drawableaxon.h"
#include "axon.h"
#include <boost/foreach.hpp>
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
#include "drawableneuron.h"
#include "drawableaxonnode.h"
#include <assert.h>
#ifdef __linux__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "glhelpfunctions.h"
#include "spikingobject.h"


bool DrawableAxon::is_applicable_to(SimulationObject* object){
    Axon* n = dynamic_cast<Axon*>(object);
    return 0 != n;
}

void DrawableAxon::set_color_and_lightning(){
    GLfloat green[] = {.6,.9,.6,0.5};
    glDisable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glColor4fv(green);
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
}

void DrawableAxon::draw_geometry_impl(){
    Axon* link = dynamic_cast<Axon*>(m_object);
    assert(link);
    Point vec = link->receiver()->position() - link->emitter()->position();
    double distance = vec.length();
    vec /= distance;
    Point start = link->emitter()->position() + vec*(DrawableAxonNode::SIZE-1);
    Point end = link->receiver()->position() - vec*(DrawableAxonNode::SIZE);

    //glBegin(GL_LINES);
    //glVertex3f(start.x,start.y,start.z);
    //glVertex3f(end.x,end.y,end.z);
    //glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex3f(start.x,start.y,start.z);
        glVertex3f(end.x,end.y,end.z);
    glEnd();
    //GLHelpFunctions::draw_cylinder(start, end, AXON_RADIUS, 32);

    //glPushMatrix();
    //glTranslatef(synapse_center.x, synapse_center.y, synapse_center.z);
    //glutSolidSphere(SYNAPSE_SIZE,20,20);
    //glPopMatrix();

    GLfloat spike_yellow[] = {1.,1.,.5,0.5};

    glColor4fv(spike_yellow);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, spike_yellow);
    BOOST_FOREACH(double spike, link->action_potentials_normalized()){
        glPushMatrix();
        Point spike_center = link->emitter()->position() + vec*(spike*distance);
        glTranslatef(spike_center.x, spike_center.y, spike_center.z);
        GLUquadricObj* obj = gluNewQuadric();
        gluSphere(obj,SPIKE_SIZE,20,20);
        gluDeleteQuadric(obj);
        glPopMatrix();
    }
}



