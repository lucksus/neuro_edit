#ifndef IZHIKEVICH_PROPERTIES_WIDGET_H
#define IZHIKEVICH_PROPERTIES_WIDGET_H

#include <QWidget>
class Izhikevich;

namespace Ui {
    class IzhikevichPropertiesWidget;
}

class IzhikevichPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IzhikevichPropertiesWidget(Izhikevich* neuron, QWidget *parent = 0);
    ~IzhikevichPropertiesWidget();

private slots:
    void on_set_button_clicked();
    void on_comboBox_activated(int type);

private:
    Ui::IzhikevichPropertiesWidget *ui;

    Izhikevich* m_neuron;
};

#endif // IZHIKEVICH_PROPERTIES_WIDGET_H
