#ifndef SAMPLES_H
#define SAMPLES_H
#include "simulationobject.h"
#include <vector>
#include <list>

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
public:
    Samples(Simulation*);
    virtual void update(double milli_seconds);

    void add_current_inducer(CurrentInducer*);
    void remove_current_inducer(CurrentInducer*);
    const std::list<CurrentInducer*>& current_inducers();
    void set_samples(const std::vector<sample>& samples);
    std::vector<sample> samples();

    void read_from_photoss_signal_file(std::string filename);

private:
    std::vector<sample> m_samples;
    std::list<CurrentInducer*> m_current_inducers;

    int find_current_index();
    int m_last_index;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SimulationObject);
        ar & boost::serialization::make_nvp("samples", m_samples);
        ar & boost::serialization::make_nvp("current_induces", m_current_inducers);
    }
};

#endif // SAMPLES_H
