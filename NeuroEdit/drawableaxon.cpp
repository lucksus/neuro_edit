#include "drawableaxon.h"
#include "axon.h"
#include <boost/foreach.hpp>
#include <gl.h>
#include "drawableneuron.h"
#include <assert.h>
#include <GLUT/glut.h>

bool DrawableAxon::is_applicable_to(SimulationObject* object){
    Axon* n = dynamic_cast<Axon*>(object);
    return 0 != n;
}

void DrawableAxon::set_color_and_lightning(){
    GLfloat gray[] = {.9,.9,.9,0.5};
    GLfloat green[] = {.0,.9,.0,0.5};
    GLfloat spike_yellow[] = {.9,.9,.0,0.5};

    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);


}

void DrawableAxon::draw_geometry(){
    Axon* link = dynamic_cast<Axon*>(m_object);
    assert(link);
    Point vec = link->receiver()->position() - link->emitter()->position();
    double distance = vec.length();
    vec /= distance;
    Point cylinder_start = link->emitter()->position() + vec*(DrawableNeuron::NEURON_SIZE-1);
    Point cylinder_end = link->receiver()->position() - vec*(DrawableNeuron::NEURON_SIZE + 1.8*SYNAPSE_SIZE);
    Point synapse_center = link->receiver()->position() - vec*(DrawableNeuron::NEURON_SIZE + SYNAPSE_SIZE);

    draw_cylinder(cylinder_start, cylinder_end, AXON_RADIUS, 32);

    glPushMatrix();
    glTranslatef(synapse_center.x, synapse_center.y, synapse_center.z);
    glutSolidSphere(SYNAPSE_SIZE,20,20);
    glPopMatrix();

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


void DrawableAxon::draw_cylinder(Point start, Point end, double radius, unsigned int slices){
    Point vec = end - start;
    double length = vec.length();
    vec /= length;

    Point plane1 = vec.orthogonal();
    Point plane2 = vec.cross(plane1);
    plane2 /= plane2.length();

    plane1 *= radius;
    plane2 *= radius;

    double pi = 3.141592653589793;
    double fac = 2*pi/slices;

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    for(unsigned int i=0; i<slices; i++){
        Point offset1 = plane1*cos(i*fac) + plane2*sin(i*fac);
        Point offset2 = plane1*cos((i+1)*fac) + plane2*sin((i+1)*fac);

        Point temp = start + offset1;
        glNormal3f(offset1.x, offset1.y, offset1.z);
        glVertex3f(temp.x,temp.y,temp.z);
        temp = start + offset2;
        glNormal3f(offset2.x, offset2.y, offset2.z);
        glVertex3f(temp.x,temp.y,temp.z);
        temp = end + offset2;
        glNormal3f(offset2.x, offset2.y, offset2.z);
        glVertex3f(temp.x,temp.y,temp.z);
        temp = end + offset1;
        glNormal3f(offset1.x, offset1.y, offset1.z);
        glVertex3f(temp.x,temp.y,temp.z);
    }
    glEnd();
}
