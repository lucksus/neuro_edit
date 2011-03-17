#ifndef NEURON_MANIPULATOR_WIDGET_H
#define NEURON_MANIPULATOR_WIDGET_H

#include <QWidget>
#include <set>
#include "current_inducer.h"

namespace Ui {
    class NeuronManipulatorWidget;
}

class DendriticNode;
class NeuronManipulatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NeuronManipulatorWidget(QWidget *parent = 0);
    ~NeuronManipulatorWidget();

public slots:
    void set_dendritic_nodes(std::set<SimulationObject*>);

private slots:
    void on_apply_potential_button_pressed();
    void on_apply_potential_button_released();

private:
    Ui::NeuronManipulatorWidget *ui;
    std::set<DendriticNode*> m_dendritic_nodes;
    std::set<CurrentInducer*> m_current_inducers;

    void create_current_inducers();
    void delete_current_inducers();
    void activate_current_inducers();
    void deactivate_current_inducers();
};

#endif // NEURON_MANIPULATOR_WIDGET_H
