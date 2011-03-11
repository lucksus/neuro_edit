#include "drawableneuron.h"
#include "neuron.h"
#include "gl.h"
#include <assert.h>
#include <GLUT/glut.h>

bool DrawableNeuron::is_applicable_to(SimulationObject* object){
    Neuron* n = dynamic_cast<Neuron*>(object);
    return 0 != n;
}

void DrawableNeuron::set_color_and_lightning(){
    Neuron* neuron = dynamic_cast<Neuron*>(m_object);
    assert(neuron);
    GLfloat transparent[] = {.9,.9,.9,0.5};
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat green[] = {.2,1, std::max(0.,neuron->membrane_potential()+70.)/100.,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
}

void DrawableNeuron::draw_geometry(){
    glutSolidSphere(NEURON_SIZE,20,20);
}
