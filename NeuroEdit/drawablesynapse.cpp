#include "drawablesynapse.h"
#include "synapse.h"
#ifdef WIN32
#include <Windows.h>
#endif
#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif
#include <boost/foreach.hpp>
#include <axon.h>
#include <assert.h>
#include "glhelpfunctions.h"
#ifdef __linux__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "math_constants.h"

bool DrawableSynapse::is_applicable_to(SimulationObject* object){
    Synapse* s = dynamic_cast<Synapse*>(object);
    return 0 != s;
}

double sigmoid(double t){
    return 1 / (1 + pow(NeuroMath::e(), -t));
}

void DrawableSynapse::set_color_and_lightning(){
    GLfloat color[] = {1.,.7,.7,0.7};
    Synapse* synapse = dynamic_cast<Synapse*>(m_object);
    assert(synapse);
    double sig = sigmoid(synapse->time_constant()/10 - 5);
    color[3] = 1-sig;
    if(synapse->weight() < 0){
        color[0] = 1.;
        color[1] = color[2] = sig;
    }else{
        color[2] = 1.;
        color[1] = color[0] = sig;
    }

    glDisable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glColor4fv(color);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
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

    double size_factor = log(abs(synapse->weight())/5);

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
