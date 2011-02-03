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
    void set_time_step(double milliseconds);
    void set_time_ratio(double ratio);

signals:
    void not_matching_speed();
    void simulation_started();
    void simulation_stopped();

protected:
    virtual void run();

private:
    bool m_stop_request;
    Network* m_network;
    double m_time_step;
    double m_time_ratio;

};

#endif // SIMULATION_H
