#include "drawableneuron.h"
#include "neuron.h"
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
#include <assert.h>
#include "dendriticnode.h"
#include <boost/foreach.hpp>
#include "drawabledendritenode.h"
#include "glhelpfunctions.h"
#include "guisettings.h"

bool DrawableNeuron::is_applicable_to(SimulationObject* object){
    Neuron* n = dynamic_cast<Neuron*>(object);
    return 0 != n;
}

void DrawableNeuron::set_color_and_lightning(){
    /*Neuron* neuron = dynamic_cast<Neuron*>(m_object);
    assert(neuron);
    GLfloat transparent[] = {.9,.9,.9,0.5};
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat green[] = {.2,1, std::max(0.,neuron->membrane_potential()+70.)/100.,.5};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    */
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat green[] = {.2,1,.7,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);

}

void DrawableNeuron::draw_geometry_impl(){
    if(GuiSettings::instance().graphics.neuron_detail < 1)
        glutSolidCube(SIZE);
    else
        GLHelpFunctions::draw_frustum(Point(-SIZE/2,0,0),Point(SIZE/2,0,0),SIZE,SIZE*2/4,4, true, true);
}
