#ifndef IZHIKEVICH_H
#define IZHIKEVICH_H
#include "neuronmodel.h"
#include <boost/serialization/base_object.hpp>
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/nvp.hpp>

class Izhikevich : public NeuronModel
{
friend class boost::serialization::access;
public:
    Izhikevich(Neuron*, double a, double b, double c, double d);
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


    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);

private:
    double m_u,m_v;
    double a,b,c,d;

    Izhikevich() {}

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        //ar & boost::serialization::base_object<NeuronModel>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NeuronModel);
        ar & BOOST_SERIALIZATION_NVP(a);
        ar & BOOST_SERIALIZATION_NVP(b);
        ar & BOOST_SERIALIZATION_NVP(c);
        ar & BOOST_SERIALIZATION_NVP(d);
        ar & BOOST_SERIALIZATION_NVP(m_u);
        ar & BOOST_SERIALIZATION_NVP(m_v);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<NeuronModel, Izhikevich>: public mpl::true_ {};
}

#endif // NEURON_H
