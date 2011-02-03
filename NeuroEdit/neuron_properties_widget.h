#ifndef NEURON_PROPERTIES_WIDGET_H
#define NEURON_PROPERTIES_WIDGET_H

#include <QWidget>
#include <set>

class SimulationObject;

namespace Ui {
    class NeuronPropertiesWidget;
}

class NeuronPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NeuronPropertiesWidget(QWidget *parent = 0);
    ~NeuronPropertiesWidget();

public slots:
    void show_properties_for_objects(std::set<SimulationObject*>);

private slots:
    void on_set_potential_button_clicked();
private:
    Ui::NeuronPropertiesWidget *ui;

    void disable();
    void enable();

    QWidget* m_model_parameters;
    std::set<SimulationObject*> m_objects;
};

#endif // NEURON_PROPERTIES_WIDGET_H
