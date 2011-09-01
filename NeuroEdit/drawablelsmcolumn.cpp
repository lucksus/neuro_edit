#include "drawablelsmcolumn.h"
#include "lsmcolumn.h"
#include <boost/foreach.hpp>
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


bool DrawableLSMColumn::is_applicable_to(SimulationObject* s){
    return dynamic_cast<LSMColumn*>(s);
}

void DrawableLSMColumn::set_color_and_lightning(){
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat color[] = {.8,0.8,0.8,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

void DrawableLSMColumn::draw_geometry_impl(){
    double min_z,max_z,min_y,max_y;
    min_z=max_z=min_y=max_y=0;
    LSMColumn* lsm_column = dynamic_cast<LSMColumn*>(m_object);
    assert(lsm_column);
    Point pos;
    Point center = lsm_column->position();
    BOOST_FOREACH(Neuron* n, lsm_column->neurons()){
        Point n_pos = n->position();
        pos = n_pos - center;
        if(pos.z < min_z) min_z = pos.z;
        if(pos.z > max_z) max_z = pos.z;
        if(pos.y < min_y) min_y = pos.y;
        if(pos.y > max_y) max_y = pos.y;
    }

    min_z -= LSMColumn::MARGIN;
    min_y -= LSMColumn::MARGIN;
    max_z += LSMColumn::MARGIN;
    max_y += LSMColumn::MARGIN;

    glPushMatrix();
    glTranslated(0,min_y,max_z);
    glutSolidCube(LSMColumn::MARGIN);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glColor3f(0.8,0.8,0.8);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    glVertex3d(0,min_y,min_z);
    glVertex3d(0,max_y,min_z);
    glVertex3d(0,max_y,max_z);
    glVertex3d(0,min_y,max_z);
    glVertex3d(0,min_y,min_z);
    glEnd();


}

QRect DrawableLSMColumn::occupied_screen_region(){
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    GLdouble model_view[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    int min_x,max_x,min_y,max_y;
    LSMColumn* lsm_column = static_cast<LSMColumn*>(m_object);
    BOOST_FOREACH(Neuron* n, lsm_column->neurons()){
        Point pos = n->position();
        GLdouble x, y, z;
        gluProject(pos.x, pos.y, pos.z, model_view, projection, viewport, &x, &y, &z);
        if(x<min_x)min_x=x;
        if(x>max_x)max_x=x;
        if(y<min_y)min_y=y;
        if(y>max_y)max_y=y;
    }
    return QRect(min_x,min_y,max_x-min_x,max_y-min_y);
}
