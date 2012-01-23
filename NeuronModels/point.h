#ifndef POINT_H
#define POINT_H
#include <math.h>
#include <boost/serialization/nvp.hpp>
#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

struct Point : public QObject{
Q_OBJECT
Q_PROPERTY(double x READ get_x WRITE set_x)
Q_PROPERTY(double y READ get_y WRITE set_y)
Q_PROPERTY(double z READ get_z WRITE set_z)
public:
    Point():QObject(){x=0;y=0;z=0;}
    Point(double x, double y, double z):QObject(){
        this->x=x;this->y=y;this->z=z;
    }
    Point(const Point& p):QObject(){
        x=p.x;y=p.y;z=p.z;
    }

    Point& operator=(const Point& p){
        x=p.x;y=p.y;z=p.z;
        return *this;
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(x);
        ar & BOOST_SERIALIZATION_NVP(y);
        ar & BOOST_SERIALIZATION_NVP(z);
    }

    static QScriptValue toScriptValue(QScriptEngine *engine, Point* const &in)
    { return engine->newQObject(in); }

    static void fromScriptValue(const QScriptValue &object, Point* &out)
    { out = qobject_cast<Point*>(object.toQObject()); }

    double x,y,z;

    inline double get_x(){return x;};
    inline double get_y(){return y;};
    inline double get_z(){return z;};
    inline void set_x(double new_x){x=new_x;};
    inline void set_y(double new_y){y=new_y;};
    inline void set_z(double new_z){z=new_z;};

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

    inline Point operator/(const double& s){
        return Point(x/s,y/s,z/s);
    }

    inline void operator/=(const double& s){
        x /= s; y /= s; z /=s;
    }

    inline double operator*(const Point& p){
        return x*p.x + y*p.y + z*p.z;
    }

    inline Point cross(const Point& p){
        return Point(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
    }

    inline Point orthogonal(){
        Point p(1,1,-(x+y)/z);
        if(0 == z) p = Point(0,0,1);
        p /= p.length();
        return p;
    }

    inline double length(){
        return distance(Point(0,0,0));
    }

public slots:
    QString toString() const {return QString("Point(x=%1,y=%2,z=%3)").arg(x).arg(y).arg(z);}
};

Q_DECLARE_METATYPE(Point)
Q_DECLARE_METATYPE(Point*)

#endif // POINT_H
