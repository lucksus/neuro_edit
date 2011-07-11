#include "drawablesynapse.h"
#include "synapse.h"
#ifdef WIN32
#include <Windows.h>
#include <GL/gl.h>
#else
#include <gl.h>
#endif
#include <boost/foreach.hpp>
#include <axon.h>
#include <assert.h>
#include "glhelpfunctions.h"
#include <GLUT/glut.h>
#include "math_constants.h"

bool DrawableSynapse::is_applicable_to(SimulationObject* object){
    Synapse* s = dynamic_cast<Synapse*>(object);
    return 0 != s;
}

double sigmoid(double t){
    return 1 / (1 + pow(NeuroMath::e(), -t));
}

void DrawableSynapse::set_color_and_lightning(){
    GLfloat green[] = {1.,.7,.7,0.7};
    Synapse* synapse = dynamic_cast<Synapse*>(m_object);
    assert(synapse);
    double sig = sigmoid(synapse->time_constant()/10 - 5);
    green[3] = 1-sig;
    green[1] = green[2] = sig;

    glDisable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glColor4fv(green);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
}

void DrawableSynapse::draw_geometry_impl(){
    Synapse* synapse = dynamic_cast<Synapse*>(m_object);
    assert(synapse);
    Point vec(0,0,0);
    BOOST_FOREACH(Axon* axon, synapse->incoming_axons()){
        vec += axon->emitter()->position() - synapse->position();
    }
    vec /= synapse->incoming_axons().size();
    vec /= vec.length();

    double size_factor = log(synapse->weight()/5);

    //GLHelpFunctions::draw_frustum(vec*START_DISTANCE,vec*END_DISTANCE,START_SIZE,END_SIZE,32, true, true);
    glPushMatrix();
    vec = vec*START_DISTANCE;
    glTranslated(vec.x,vec.y,vec.z);
    glutSolidSphere(SIZE_AT_WEIGHT_30*size_factor, 5,5);
    glPopMatrix();
    //glBegin(GL_POINTS);
    //glVertex3f(vec.x, vec.y, vec.z);
    //glEnd();
}


Point DrawableSynapse::displacement(Synapse* synapse){
    Point vec(0,0,0);
    BOOST_FOREACH(Axon* axon, synapse->incoming_axons()){
        vec += axon->emitter()->position() - synapse->position();
    }
    vec /= synapse->incoming_axons().size();
    vec /= vec.length();

    return vec * (START_DISTANCE+END_DISTANCE)/2;

}
