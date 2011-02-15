#ifndef POINT_H
#define POINT_H
#include <math.h>

struct Point{
    Point(){x=0;y=0;z=0;}
    Point(double x, double y, double z){
        this->x=x;this->y=y;this->z=z;
    }

    double x,y,z;
    inline double distance(Point p){
        Point diff = p - *this;
        return sqrt(pow((diff.x),2) + pow((diff.y),2) + pow((diff.z),2));
    }

    inline Point operator-(const Point& p){
        return Point(x-p.x,y-p.y,z-p.z);
    }

    inline Point operator+(const Point& p){
        return Point(x+p.x,y+p.y,z+p.z);
    }

    inline void operator+=(const Point& p){
        x += p.x; y += p.y; z += p.z;
    }

    inline Point operator*(const double& s){
        return Point(x*s,y*s,z*s);
    }

    inline void operator*=(const double& s){
        x *= s; y *= s; z *=s;
    }

    inline void operator/=(const double& s){
        x /= s; y /= s; z /=s;
    }

    inline double operator*(const Point& p){
        return x*p.x + y*p.y + z*p.z;
    }

    inline double length(){
        return distance(Point(0,0,0));
    }
};

#endif // POINT_H
