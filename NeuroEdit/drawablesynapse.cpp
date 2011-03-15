#include "drawablesynapse.h"
#include "synapse.h"
#include <gl.h>
#include <boost/foreach.hpp>
#include <axon.h>
#include <assert.h>
#include <GLUT/glut.h>
#include "glhelpfunctions.h"

bool DrawableSynapse::is_applicable_to(SimulationObject* object){
    Synapse* s = dynamic_cast<Synapse*>(object);
    return 0 != s;
}

void DrawableSynapse::set_color_and_lightning(){
    GLfloat green[] = {.0,.9,.5,1};
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
}

void DrawableSynapse::draw_geometry(){
    Synapse* synapse = dynamic_cast<Synapse*>(m_object);
    assert(synapse);
    Point vec(0,0,0);
    BOOST_FOREACH(Axon* axon, synapse->incoming_axons()){
        vec += axon->emitter()->position() - synapse->position();
    }
    vec /= synapse->incoming_axons().size();
    vec /= vec.length();

    GLHelpFunctions::draw_frustum(vec*START_DISTANCE,vec*END_DISTANCE,START_SIZE,END_SIZE,32, true, true);
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
