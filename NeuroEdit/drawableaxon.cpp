#include "drawableaxon.h"
#include "axon.h"
#include <boost/foreach.hpp>
#include <gl.h>
#include "drawableneuron.h"
#include "drawableaxonnode.h"
#include <assert.h>
#include <GLUT/glut.h>
#include "glhelpfunctions.h"
#include "spikingobject.h"

bool DrawableAxon::is_applicable_to(SimulationObject* object){
    Axon* n = dynamic_cast<Axon*>(object);
    return 0 != n;
}

void DrawableAxon::set_color_and_lightning(){
    GLfloat green[] = {.0,.9,.0,0.5};
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
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

    GLHelpFunctions::draw_cylinder(start, end, AXON_RADIUS, 32);

    //glPushMatrix();
    //glTranslatef(synapse_center.x, synapse_center.y, synapse_center.z);
    //glutSolidSphere(SYNAPSE_SIZE,20,20);
    //glPopMatrix();

    GLfloat spike_yellow[] = {.9,.9,.0,0.5};

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, spike_yellow);
    BOOST_FOREACH(double spike, link->action_potentials_normalized()){
        glPushMatrix();
        Point spike_center = link->emitter()->position() + vec*(spike*distance);
        glTranslatef(spike_center.x, spike_center.y, spike_center.z);
        glutSolidSphere(SPIKE_SIZE,20,20);
        glPopMatrix();
    }
}



