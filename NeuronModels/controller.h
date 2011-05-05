#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QtCore/QObject>

class Simulation;
class Controller : public QObject
{
Q_OBJECT
public:
    static Controller& instance();

    void init();

    void close_simulation();
    void create_new_simulation();
    void load_simulation(std::string filename);
    void save_simulation(std::string filename);

    Simulation* simulation();

signals:
    void new_simulation(Simulation*);

private:
    Controller();
    Simulation* m_simulation;
};

#endif // CONTROLLER_H
