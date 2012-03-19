#ifndef SAMPLES_H
#define SAMPLES_H
#include "simulationobject.h"
#include <vector>
#include <list>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include "current_inducer.h"
#include "lineardiscriminator.h"

struct sample{
    double time;
    double value;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("time", time);
        ar & boost::serialization::make_nvp("value", value);
    }
};

class CurrentInducer;

class Samples : public SimulationObject
{
Q_OBJECT
Q_PROPERTY(bool constant_value_active READ constant_value_active WRITE set_constant_value_active)
Q_PROPERTY(double constant_value READ constant_value WRITE set_constant_value)
friend class boost::serialization::access;
public:
    Samples(Simulation*);
    Samples(){};
    virtual void update(double milli_seconds);

    Q_INVOKABLE void add_current_inducer(CurrentInducer*);
    Q_INVOKABLE void remove_current_inducer(CurrentInducer*);
    const std::list<CurrentInducer*>& current_inducers();

    void add_linear_discriminator(LinearDiscriminator*);
    void remove_linear_discriminator(LinearDiscriminator*);
    const std::set<LinearDiscriminator*>& linear_discriminators();

    void set_samples(const std::vector<sample>& samples);
    std::vector<sample> samples();

    Q_INVOKABLE void read_from_photoss_signal_file(QString filename) {read_from_photoss_signal_file(filename.toStdString());}
    void read_from_photoss_signal_file(std::string filename);

    virtual std::list<std::string> user_actions();
    virtual void do_user_action(std::string);

    void write_value(double value);

    double value();

    Q_INVOKABLE void set_constant_value_active(bool);
    Q_INVOKABLE bool constant_value_active();
    Q_INVOKABLE void set_constant_value(double);
    Q_INVOKABLE double constant_value();
    Q_INVOKABLE void clear();


private:

    std::vector<sample> m_samples;
    std::list<CurrentInducer*> m_current_inducers;
    std::set<LinearDiscriminator*> m_linear_discriminators;

    int find_current_index();
    int m_last_index;
    double m_last_time;
    double m_current_value;
    bool m_constant_value_active;
    double m_constant_value;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("samples", m_samples);
        ar & boost::serialization::make_nvp("current_induces", m_current_inducers);
        ar & boost::serialization::make_nvp("linear_discriminators", m_linear_discriminators);
        ar & boost::serialization::make_nvp("constant_value_active", m_constant_value_active);
        ar & boost::serialization::make_nvp("constant_value", m_constant_value);
    }

    static unsigned int s_serial;
};

namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, Samples>: public mpl::true_ {};
}

Q_DECLARE_METATYPE(Samples)
Q_DECLARE_METATYPE(Samples*)

#endif // SAMPLES_H
