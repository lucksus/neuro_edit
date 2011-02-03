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

private:
    Ui::NeuronPropertiesWidget *ui;

    void disable();
    void enable();

    QWidget* m_model_parameters;
};

#endif // NEURON_PROPERTIES_WIDGET_H
