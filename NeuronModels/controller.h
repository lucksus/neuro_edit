#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <list>
#include <QMutex>

class Simulation;
class ScriptEngine;
class Controller : public QObject
{
Q_OBJECT
public:
    static Controller& instance();

    void init();

    Q_INVOKABLE void close_simulation();
    Q_INVOKABLE void create_new_simulation();
    void load_simulation(std::string filename);
    void save_simulation(std::string filename);
    Q_INVOKABLE void save_simulation();
    Q_INVOKABLE void load_simulation(QString filename);
    Q_INVOKABLE void save_simulation(QString filename);

    void export_xml_simulation(std::string);
    void import_xml_simulation(std::string);

    Q_INVOKABLE Simulation* simulation();
    ScriptEngine* script_engine();

    std::list<QString> m_script_output_buffer;
    QMutex m_mutex_for_buffer;

    std::string version_string(){return "0.1";}
    bool std_output_activated();
    void set_std_output(bool);

public slots:
    void output_from_script(QString);

signals:
    void new_simulation(Simulation*);
    void script_output(QString);

private:
    Controller();
    Simulation* m_simulation;
    std::string m_simulation_filename;
    ScriptEngine* m_script_engine;
    bool m_std_output_activated;
};

#endif // CONTROLLER_H
