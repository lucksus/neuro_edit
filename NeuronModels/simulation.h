#ifndef SIMULATION_H
#define SIMULATION_H
#include <QtCore/QThread>

class Network;

class Simulation : public QThread
{
Q_OBJECT
public:
    Simulation();
    void request_stop();

    void set_network(Network*);
    void set_simulation_step(double milliseconds);
    void set_real_step(double milliseconds);

    double simulation_step();
    double real_step();

signals:
    void not_matching_speed();
    void simulation_started();
    void simulation_stopped();
    void simulation_time_passed(long useconds);

protected:
    virtual void run();

private:
    bool m_stop_request;
    Network* m_network;
    double m_simulation_step;
    double m_real_step;

};

#endif // SIMULATION_H
