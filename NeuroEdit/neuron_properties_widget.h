#ifndef NEURON_PROPERTIES_WIDGET_H
#define NEURON_PROPERTIES_WIDGET_H

#include <QWidget>

namespace Ui {
    class NeuronPropertiesWidget;
}

class NeuronPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NeuronPropertiesWidget(QWidget *parent = 0);
    ~NeuronPropertiesWidget();

private:
    Ui::NeuronPropertiesWidget *ui;
};

#endif // NEURON_PROPERTIES_WIDGET_H
