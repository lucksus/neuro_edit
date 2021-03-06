#include "drawabledendritenode.h"
#include "dendriticnode.h"
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
#include "glhelpfunctions.h"

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

    DendriticNode* n = dynamic_cast<DendriticNode*>(m_object);
    assert(n);
    DendriticNode* parent = n->parent();
    if(!parent) return;
    Point relative = parent->position() - n->position();

    GLHelpFunctions::draw_cylinder(Point(0,0,0), relative, 3, 4);
}
