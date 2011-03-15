#include "drawabledendriteconnection.h"
#include "dendriticnode.h"
#include <gl.h>
#include <GLUT/glut.h>
#include <assert.h>
#include "glhelpfunctions.h"

bool DrawableDendriteConnection::is_applicable_to(SimulationObject* object){
    DendriticNode* n = dynamic_cast<DendriticNode*>(object);
    return 0 != n;
}

void DrawableDendriteConnection::set_color_and_lightning(){
    //glEnable(GL_DITHER);
    //glDisable(GL_LIGHTING);
    //glColor3f(1,1,1);

    GLfloat green[] = {.0,.9,.6,0.8};
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
}

void DrawableDendriteConnection::draw_geometry(){
    DendriticNode* n = dynamic_cast<DendriticNode*>(m_object);
    assert(n);
    DendriticNode* parent = n->parent();
    if(!parent) return;
    Point relative = parent->position() - n->position();

    GLHelpFunctions::draw_cylinder(Point(0,0,0), relative, 3, 4);

}
