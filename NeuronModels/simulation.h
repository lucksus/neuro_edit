#ifndef SIMULATION_H
#define SIMULATION_H
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include "scriptengine.h"
#include <QtCore/QStringList>

class Network;
class Simulation : public QThread
{
Q_OBJECT
Q_PROPERTY(Network* network READ network WRITE set_network)
Q_PROPERTY(double simulation_step READ simulation_step WRITE set_simulation_step)
Q_PROPERTY(double real_step READ real_step WRITE set_real_step)
Q_PROPERTY(double time READ time_ms)
friend class boost::serialization::access;
public:
    Simulation();
    Q_INVOKABLE void request_stop();
    Q_INVOKABLE void wait_till_finished();

    enum FileFormat{BINARY,XML};

    void write_to_file(const std::string& filename, FileFormat format);
    Q_INVOKABLE void write_to_file(const QString& filename);
    static Simulation* load_from_file(const std::string& filename, FileFormat format);

    void set_network(Network*);
    Network* network();
    void set_simulation_step(double milliseconds);
    void set_real_step(double milliseconds);
    void set_filename(const std::string&);
    std::string filename();

    double simulation_step();
    double real_step();
    double time_ms();

    Q_INVOKABLE QStringList scripts() const;
    Q_INVOKABLE QString script(const QString&) const;
    Q_INVOKABLE void set_script(const QString& name, const QString& script);
    Q_INVOKABLE QString run_script(const QString& name);
    Q_INVOKABLE void remove_script(const QString& name);
    Q_INVOKABLE bool is_running();

signals:
    void not_matching_speed();
    void simulation_started();
    void simulation_stopped();
    void simulation_milliseconds_passed(double milliseconds);

protected:
    virtual void run();

private:
    bool m_stop_request;
    bool m_is_running;
    Network* m_network;
    double m_simulation_step;
    double m_real_step;
    double m_time_ms;
    std::map<std::string, std::string> m_scripts;
    QMutex m_mutex;
    std::string m_filename;
    ScriptEngine m_script_engine;


    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("Network",m_network);
        ar & boost::serialization::make_nvp("Simulation_Step",m_simulation_step);
        ar & boost::serialization::make_nvp("Real_Step",m_real_step);
        ar & boost::serialization::make_nvp("Time",m_time_ms);
        ar & boost::serialization::make_nvp("Scripts", m_scripts);
    }

};

#endif // SIMULATION_H
