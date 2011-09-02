#include "drawableldconnection.h"
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
#include <assert.h>
#ifdef __linux__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "glhelpfunctions.h"
#include "ldconnection.h"
#include "lineardiscriminator.h"
#include "drawablelineardiscriminator.h"

bool DrawableLDConnection::is_applicable_to(SimulationObject* object){
    LDConnection* n = dynamic_cast<LDConnection*>(object);
    return 0 != n;
}

void DrawableLDConnection::set_color_and_lightning(){
    LDConnection* conn = dynamic_cast<LDConnection*>(m_object);
    double w = conn->weight();
    double red = 0.6;
    double green = 0.6;
    if(w>0){
        green += 0.1 + w/10;
    }else{
        red += 0.1 + w/10;
    }
    GLfloat color[] = {red,green,.6,1};
    glDisable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glColor4fv(color);
}

void DrawableLDConnection::draw_geometry_impl(){
    LDConnection* link = dynamic_cast<LDConnection*>(m_object);
    assert(link);
    Point vec = link->pre_neuron()->position() - link->post_neuron()->position();
    double distance = vec.length();
    vec /= distance;
    Point start = link->pre_neuron()->position();
    Point end = link->post_neuron()->position();

    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex3f(start.x,start.y,start.z);
        glVertex3f(end.x,end.y,end.z);
    glEnd();

    Point synapse = end + vec * (DrawableLinearDiscriminator::SIZE*1.2);
    glPushMatrix();
    glTranslated(synapse.x, synapse.y, synapse.z);
    glutSolidSphere(DrawableLinearDiscriminator::SIZE*0.3,10,5);
    glPopMatrix();
}
