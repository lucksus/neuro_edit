#ifndef SIMULATION_H
#define SIMULATION_H
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>

class Network;

class Simulation : public QThread
{
Q_OBJECT
friend class boost::serialization::access;
public:
    Simulation();
    void request_stop();
    void wait_till_finished();

    void write_to_file(const std::string& filename);
    Q_INVOKABLE void write_to_file(const QString& filename);
    static Simulation* load_from_file(const std::string& filename);

    void set_network(Network*);
    Network* network();
    void set_simulation_step(double milliseconds);
    void set_real_step(double milliseconds);

    double simulation_step();
    double real_step();

    double time_ms();

signals:
    void not_matching_speed();
    void simulation_started();
    void simulation_stopped();
    void simulation_milliseconds_passed(double milliseconds);

protected:
    virtual void run();

private:
    bool m_stop_request;
    Network* m_network;
    double m_simulation_step;
    double m_real_step;
    double m_time_ms;
    QMutex m_mutex;


    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("Network",m_network);
        ar & boost::serialization::make_nvp("Simulation_Step",m_simulation_step);
        ar & boost::serialization::make_nvp("Real_Step",m_real_step);
        ar & boost::serialization::make_nvp("Time",m_time_ms);
    }

};

#endif // SIMULATION_H
