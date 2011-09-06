#include "drawablesamples.h"
#include "samples.h"
#include "glhelpfunctions.h"
#ifdef __linux__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <boost/foreach.hpp>
#include <assert.h>
#include "current_inducer.h"

DrawableSamples::DrawableSamples()
{
    dont_use_display_lists();
}


bool DrawableSamples::is_applicable_to(SimulationObject* object){
    Samples* n = dynamic_cast<Samples*>(object);
    return 0 != n;
}

void DrawableSamples::set_color_and_lightning(){
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat color[] = {.75,0.,0.,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

void DrawableSamples::draw_geometry_impl(){
    glutSolidCube(SIZE);

    Samples* samples = dynamic_cast<Samples*>(m_object);
    assert(samples);
    glDisable(GL_LIGHTING);
    glColor4f(1.,0.,0.,0.7);
    glBegin(GL_LINES);
    BOOST_FOREACH(CurrentInducer* current_inducer, samples->current_inducers()){
        glVertex3d(0.,0.,0.);
        Point vec = current_inducer->position() - samples->position() ;
        glVertex3d(vec.x, vec.y, vec.z);
    }
    BOOST_FOREACH(LinearDiscriminator* ld, samples->linear_discriminators()){
        glVertex3d(0.,0.,0.);
        Point vec = ld->position() - samples->position() ;
        glVertex3d(vec.x, vec.y, vec.z);
    }
    glEnd();
}

