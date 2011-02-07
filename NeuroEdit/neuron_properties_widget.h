#ifndef NEURON_PROPERTIES_WIDGET_H
#define NEURON_PROPERTIES_WIDGET_H

#include <QWidget>
#include <set>



namespace Ui {
    class NeuronPropertiesWidget;
}

class CurrentInducer;
class Network;
class SimulationObject;
class NeuronPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NeuronPropertiesWidget(QWidget *parent = 0);
    ~NeuronPropertiesWidget();

    void set_network(Network* net);

public slots:
    void show_properties_for_objects(std::set<SimulationObject*>);
    void simulation_started();
    void simulation_stopped();

private slots:
    void on_set_potential_button_clicked();
    void on_add_potential_button_pressed();
    void on_add_potential_button_released();
private:
    Ui::NeuronPropertiesWidget *ui;

    void disable();
    void enable();

    QWidget* m_model_parameters;
    std::set<SimulationObject*> m_objects;

    bool m_simulation_running;
    Network* m_network;
    std::set<CurrentInducer*> m_current_inducers;

    void create_current_inducers();
    void delete_current_inducers();
    void activate_current_inducers();
    void deactivate_current_inducers();
};

#endif // NEURON_PROPERTIES_WIDGET_H
