#include "drawabledendritenode.h"
#include "dendriticnode.h"
#include <gl.h>
#include <GLUT/glut.h>
#include <assert.h>

bool DrawableDendriteNode::is_applicable_to(SimulationObject* object){
    DendriticNode* n = dynamic_cast<DendriticNode*>(object);
    return 0 != n;
}

void DrawableDendriteNode::set_color_and_lightning(){
    GLfloat green[] = {.0,.9,.6,0.8};
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
}

void DrawableDendriteNode::draw_geometry_impl(){
    glutSolidCube(SIZE);
}
