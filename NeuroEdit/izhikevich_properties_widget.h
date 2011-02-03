#ifndef IZHIKEVICH_PROPERTIES_WIDGET_H
#define IZHIKEVICH_PROPERTIES_WIDGET_H

#include <QWidget>

namespace Ui {
    class IzhikevichPropertiesWidget;
}

class IzhikevichPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IzhikevichPropertiesWidget(QWidget *parent = 0);
    ~IzhikevichPropertiesWidget();

private:
    Ui::IzhikevichPropertiesWidget *ui;
};

#endif // IZHIKEVICH_PROPERTIES_WIDGET_H
