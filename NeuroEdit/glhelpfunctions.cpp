#include "glhelpfunctions.h"
#include "gl.h"

void GLHelpFunctions::draw_cylinder(Point start, Point end, double radius, unsigned int slices){
    Point vec = end - start;
    double length = vec.length();
    vec /= length;

    Point plane1 = vec.orthogonal();
    Point plane2 = vec.cross(plane1);
    plane2 /= plane2.length();

    plane1 *= radius;
    plane2 *= radius;

    double pi = 3.141592653589793;
    double fac = 2*pi/slices;

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    for(unsigned int i=0; i<slices; i++){
        Point offset1 = plane1*cos(i*fac) + plane2*sin(i*fac);
        Point offset2 = plane1*cos((i+1)*fac) + plane2*sin((i+1)*fac);

        Point temp = start + offset1;
        glNormal3f(offset1.x, offset1.y, offset1.z);
        glVertex3f(temp.x,temp.y,temp.z);
        temp = start + offset2;
        glNormal3f(offset2.x, offset2.y, offset2.z);
        glVertex3f(temp.x,temp.y,temp.z);
        temp = end + offset2;
        glNormal3f(offset2.x, offset2.y, offset2.z);
        glVertex3f(temp.x,temp.y,temp.z);
        temp = end + offset1;
        glNormal3f(offset1.x, offset1.y, offset1.z);
        glVertex3f(temp.x,temp.y,temp.z);
    }
    glEnd();
}


void GLHelpFunctions::draw_frustum(Point start, Point end, double start_radius, double end_radius, unsigned int slices, bool bottom, bool top, bool smooth){
    Point vec = end - start;
    double length = vec.length();
    vec /= length;

    Point plane1 = vec.orthogonal();
    Point plane2 = vec.cross(plane1);
    plane2 /= plane2.length();

    //plane1 *= radius;
    //plane2 *= radius;

    double pi = 3.141592653589793;
    double fac = 2*pi/slices;

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    for(unsigned int i=0; i<slices; i++){
        Point offset1 = plane1*cos(i*fac) + plane2*sin(i*fac);
        Point offset2 = plane1*cos((i+1)*fac) + plane2*sin((i+1)*fac);

        Point normal = offset1 + offset2;
        glNormal3f(normal.x, normal.y, normal.z);

        Point temp = start + offset1*start_radius;
        if(smooth) glNormal3f(offset1.x, offset1.y, offset1.z);
        glVertex3f(temp.x,temp.y,temp.z);

        temp = start + offset2*start_radius;
        if(smooth) glNormal3f(offset2.x, offset2.y, offset2.z);
        glVertex3f(temp.x,temp.y,temp.z);

        temp = end + offset2*end_radius;
        if(smooth) glNormal3f(offset2.x, offset2.y, offset2.z);
        glVertex3f(temp.x,temp.y,temp.z);

        temp = end + offset1*end_radius;
        if(smooth) glNormal3f(offset1.x, offset1.y, offset1.z);
        glVertex3f(temp.x,temp.y,temp.z);


    }
    glEnd();


    if(bottom){
        glBegin(GL_TRIANGLES);
        Point normal = start - end;
        glNormal3f(normal.x, normal.y, normal.z);
        for(unsigned int i=0; i<slices; i++){
            Point offset1 = plane1*cos(i*fac) + plane2*sin(i*fac);
            Point offset2 = plane1*cos((i+1)*fac) + plane2*sin((i+1)*fac);

            Point temp;
            temp = start + offset2*start_radius;
            glVertex3f(temp.x,temp.y,temp.z);
            temp = start + offset1*start_radius;
            glVertex3f(temp.x,temp.y,temp.z);
            glVertex3f(start.x, start.y, start.z);

        }
        glEnd();
    }

    if(top){
        glBegin(GL_TRIANGLES);
        Point normal = end - start;
        glNormal3f(normal.x, normal.y, normal.z);
        for(unsigned int i=0; i<slices; i++){
            Point offset1 = plane1*cos(i*fac) + plane2*sin(i*fac);
            Point offset2 = plane1*cos((i+1)*fac) + plane2*sin((i+1)*fac);

            Point temp = end + offset1*end_radius;
            glVertex3f(end.x, end.y, end.z);
            glVertex3f(temp.x,temp.y,temp.z);
            temp = end + offset2*end_radius;
            glVertex3f(temp.x,temp.y,temp.z);
        }
        glEnd();
    }
}
