#ifndef IZHIKEVICH_H
#define IZHIKEVICH_H
#include "neuron.h"

class Izhikevich : public Neuron
{
public:
    Izhikevich(Point position, double a, double b, double c, double d);

    virtual bool update(double milli_seconds, double current);
    virtual double membrane_potential();
    virtual bool is_spiking();

    inline double get_a(){return a;}
    inline double get_b(){return b;}
    inline double get_c(){return c;}
    inline double get_d(){return d;}

    void set_a(double x){a=x;}
    void set_b(double x){b=x;}
    void set_c(double x){c=x;}
    void set_d(double x){d=x;}

private:
    double u,v;
    double a,b,c,d;
    bool m_spiking;
};

#endif // NEURON_H
