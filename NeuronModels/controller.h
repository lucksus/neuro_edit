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
    void save_simulation();

    Simulation* simulation();

public slots:
    void output_from_script(QString);

signals:
    void new_simulation(Simulation*);
    void script_output(QString);

private:
    Controller();
    Simulation* m_simulation;
    std::string m_simulation_filename;
};

#endif // CONTROLLER_H
