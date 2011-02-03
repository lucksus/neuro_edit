#include "izhikevich_properties_widget.h"
#include "ui_izhikevich_properties_widget.h"

IzhikevichPropertiesWidget::IzhikevichPropertiesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IzhikevichPropertiesWidget)
{
    ui->setupUi(this);
}

IzhikevichPropertiesWidget::~IzhikevichPropertiesWidget()
{
    delete ui;
}
