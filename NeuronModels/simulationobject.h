#ifndef SIMULATIONOBJECT_H
#define SIMULATIONOBJECT_H
#include <boost/any.hpp>
#include <map>

class SimulationObject
{
public:
    virtual void update(double milli_seconds) = 0;
    virtual SimulationObject* clone() = 0;

    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);
    virtual void set_properties(std::map<std::string, boost::any);
};

#endif // SIMULATIONOBJECT_H
