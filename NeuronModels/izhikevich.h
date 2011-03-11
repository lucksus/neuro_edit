#ifndef IZHIKEVICH_H
#define IZHIKEVICH_H
#include "neuronmodel.h"

class Izhikevich : public NeuronModel
{
public:
    Izhikevich(double a, double b, double c, double d);
    Izhikevich(const Izhikevich&);

    virtual void update(double milli_seconds);
    virtual double membrane_potential();
    virtual void set_membrane_potential(double);

    inline double get_a(){return a;}
    inline double get_b(){return b;}
    inline double get_c(){return c;}
    inline double get_d(){return d;}

    void set_a(double x){a=x;}
    void set_b(double x){b=x;}
    void set_c(double x){c=x;}
    void set_d(double x){d=x;}

    double v();
    double u();

private:
    double m_u,m_v;
    double a,b,c,d;
};

#endif // NEURON_H
