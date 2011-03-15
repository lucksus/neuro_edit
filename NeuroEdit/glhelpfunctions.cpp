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
