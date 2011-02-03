#include "izhikevich_properties_widget.h"
#include "ui_izhikevich_properties_widget.h"

IzhikevichPropertiesWidget::IzhikevichPropertiesWidget(Izhikevich* neuron, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IzhikevichPropertiesWidget),
    m_neuron(neuron)
{
    ui->setupUi(this);
}

IzhikevichPropertiesWidget::~IzhikevichPropertiesWidget()
{
    delete ui;
}
