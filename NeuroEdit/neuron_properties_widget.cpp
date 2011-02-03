#include "neuron_properties_widget.h"
#include "ui_neuron_properties_widget.h"

NeuronPropertiesWidget::NeuronPropertiesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeuronPropertiesWidget)
{
    ui->setupUi(this);
}

NeuronPropertiesWidget::~NeuronPropertiesWidget()
{
    delete ui;
}
