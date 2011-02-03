#ifndef IZHIKEVICH_PROPERTIES_WIDGET_H
#define IZHIKEVICH_PROPERTIES_WIDGET_H

#include <QWidget>
#include <set>
class Izhikevich;

namespace Ui {
    class IzhikevichPropertiesWidget;
}

class IzhikevichPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    IzhikevichPropertiesWidget(Izhikevich* neuron, QWidget *parent = 0);
    IzhikevichPropertiesWidget(std::set<Izhikevich*> neurons, QWidget *parent = 0);
    ~IzhikevichPropertiesWidget();

private slots:
    void on_set_button_clicked();
    void on_comboBox_activated(int type);

private:
    Ui::IzhikevichPropertiesWidget *ui;

    Izhikevich* m_neuron;
    std::set<Izhikevich*> m_neurons;
};

#endif // IZHIKEVICH_PROPERTIES_WIDGET_H
