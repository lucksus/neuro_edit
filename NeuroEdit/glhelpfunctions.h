#ifndef GLHELPFUNCTIONS_H
#define GLHELPFUNCTIONS_H
#include "point.h"

class GLHelpFunctions
{
public:
    static void draw_cylinder(Point start, Point end, double radius, unsigned int slices);
    static void draw_frustum(Point start, Point end, double start_radius, double end_radius, unsigned int slices, bool bottom, bool top, bool smooth = false);
};

#endif // GLHELPFUNCTIONS_H
