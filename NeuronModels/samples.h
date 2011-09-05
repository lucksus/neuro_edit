#ifndef SAMPLES_H
#define SAMPLES_H
#include "simulationobject.h"
#include <vector>
#include <list>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include "current_inducer.h"

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
friend class boost::serialization::access;
public:
    Samples(Simulation*);
    Samples(){};
    virtual void update(double milli_seconds);

    void add_current_inducer(CurrentInducer*);
    void remove_current_inducer(CurrentInducer*);
    const std::list<CurrentInducer*>& current_inducers();
    void set_samples(const std::vector<sample>& samples);
    std::vector<sample> samples();

    void read_from_photoss_signal_file(std::string filename);

    virtual std::list<std::string> user_actions();
    virtual void do_user_action(std::string);

private:

    std::vector<sample> m_samples;
    std::list<CurrentInducer*> m_current_inducers;

    int find_current_index();
    int m_last_index;
    double m_last_time;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("samples", m_samples);
        ar & boost::serialization::make_nvp("current_induces", m_current_inducers);
    }
};

namespace boost{
template<>
struct is_virtual_base_of<SimulationObject, Samples>: public mpl::true_ {};
}

Q_DECLARE_METATYPE(Samples)
Q_DECLARE_METATYPE(Samples*)

#endif // SAMPLES_H
