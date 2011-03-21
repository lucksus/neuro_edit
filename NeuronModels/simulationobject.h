#ifndef SIMULATIONOBJECT_H
#define SIMULATIONOBJECT_H
#include <set>
#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>

class Neuron;
class SimulationObject
{
friend class boost::serialization::access;
public:
    SimulationObject(Neuron*);
    virtual ~SimulationObject(){}

    virtual void update(double milli_seconds) = 0;
    virtual std::set<SimulationObject*> children();
    virtual Neuron* neuron();
    //virtual SimulationObject* clone() = 0;

    bool is_done();
    void reset_done();

protected:
    SimulationObject(){}
    void done();

private:
    Neuron* m_neuron;
    bool m_done;


    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(m_neuron);
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(SimulationObject);

#endif // SIMULATIONOBJECT_H
