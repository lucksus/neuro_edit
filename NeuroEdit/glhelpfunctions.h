#ifndef GLHELPFUNCTIONS_H
#define GLHELPFUNCTIONS_H
#include "point.h"

class GLHelpFunctions
{
public:
    static void draw_cylinder(Point start, Point end, double radius, unsigned int slices);
};

#endif // GLHELPFUNCTIONS_H
