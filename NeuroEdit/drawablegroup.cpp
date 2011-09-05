#include "drawablegroup.h"
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
#include "group.h"

bool DrawableGroup::is_applicable_to(SimulationObject* s){
    return dynamic_cast<Group*>(s);
}

void DrawableGroup::set_color_and_lightning(){
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    GLfloat color[] = {.8,0.8,0.8,1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

void DrawableGroup::draw_geometry_impl(){
    double min_z,max_z,min_y,max_y,min_x,max_x;
    min_z=max_z=min_y=max_y=min_x=max_x=0;
    Group* group = dynamic_cast<Group*>(m_object);
    Point pos;
    Point center = group->position();
    BOOST_FOREACH(SimulationObject* o, group->objects_as_std_set()){
        Point n_pos = o->position();
        pos = n_pos - center;
        if(pos.z < min_z) min_z = pos.z;
        if(pos.z > max_z) max_z = pos.z;
        if(pos.y < min_y) min_y = pos.y;
        if(pos.y > max_y) max_y = pos.y;
        if(pos.x < min_x) min_x = pos.x;
        if(pos.x > max_x) max_x = pos.x;
    }

    min_z -= Group::MARGIN;
    min_y -= Group::MARGIN;
    min_x -= Group::MARGIN;
    max_z += Group::MARGIN;
    max_y += Group::MARGIN;
    max_x += Group::MARGIN;

    glPushMatrix();
    Point handle = group->handle_position();
    glTranslated(handle.x,handle.y,handle.z);
    glutSolidCube(Group::MARGIN);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glColor3f(0.8,0.8,0.8);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    if(group->drawn_horizontally()){
        glVertex3d(min_x,0,min_z);
        glVertex3d(max_x,0,min_z);
        glVertex3d(max_x,0,max_z);
        glVertex3d(min_x,0,max_z);
        glVertex3d(min_x,0,min_z);
    }else{
        glVertex3d(0,min_y,min_z);
        glVertex3d(0,max_y,min_z);
        glVertex3d(0,max_y,max_z);
        glVertex3d(0,min_y,max_z);
        glVertex3d(0,min_y,min_z);
    }
    glEnd();

    glColor3f(0.8,0.0,0.0);
    glLineWidth(1);
    glBegin(GL_LINES);
    BOOST_FOREACH(Samples* samples, group->inputs()){
        glVertex3d(handle.x, handle.y, handle.z);
        Point vec = samples->position() - group->position();
        glVertex3d(vec.x, vec.y, vec.z);
    }
    glEnd();


}

QRect DrawableGroup::occupied_screen_region(){
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    GLdouble model_view[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    int min_x,max_x,min_y,max_y;
    Group* group = static_cast<Group*>(m_object);
    BOOST_FOREACH(SimulationObject* o, group->objects()){
        Point pos = o->position();
        GLdouble x, y, z;
        gluProject(pos.x, pos.y, pos.z, model_view, projection, viewport, &x, &y, &z);
        if(x<min_x)min_x=x;
        if(x>max_x)max_x=x;
        if(y<min_y)min_y=y;
        if(y>max_y)max_y=y;
    }
    return QRect(min_x,min_y,max_x-min_x,max_y-min_y);
}
