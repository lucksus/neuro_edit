#include "drawabledendriteconnection.h"
#include "dendriticnode.h"
#include <gl.h>
#include <GLUT/glut.h>
#include <assert.h>

bool DrawableDendriteConnection::is_applicable_to(SimulationObject* object){
    DendriticNode* n = dynamic_cast<DendriticNode*>(object);
    return 0 != n;
}

void DrawableDendriteConnection::set_color_and_lightning(){
    glEnable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
}

void DrawableDendriteConnection::draw_geometry(){
    DendriticNode* n = dynamic_cast<DendriticNode*>(m_object);
    assert(n);
    DendriticNode* parent = n->parent();
    if(!parent) return;
    Point relative = parent->position() - n->position();

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(relative.x,relative.y,relative.z);
    glEnd();
}
