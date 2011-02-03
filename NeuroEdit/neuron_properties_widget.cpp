#include "neuron_properties_widget.h"
#include "ui_neuron_properties_widget.h"
#include "simulationobject.h"
#include "izhikevich.h"
#include "izhikevich_properties_widget.h"

NeuronPropertiesWidget::NeuronPropertiesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeuronPropertiesWidget),
    m_model_parameters(0)
{
    ui->setupUi(this);
    ui->modelParameters->setLayout(new QHBoxLayout());
    disable();
}

NeuronPropertiesWidget::~NeuronPropertiesWidget()
{
    delete ui;
}


void NeuronPropertiesWidget::show_properties_for_objects(std::set<SimulationObject *> objects){
    disable();
    if(objects.empty() || objects.size() > 1){
        return;
    }

    SimulationObject* o = *(objects.begin());
    Izhikevich* izhikevich_neuron = dynamic_cast<Izhikevich*>(o);

    if(izhikevich_neuron){
        ui->type->setText("Izhikevich");
        enable();
        m_model_parameters = new IzhikevichPropertiesWidget(izhikevich_neuron);
        ui->modelParameters->layout()->addWidget(m_model_parameters);
    }

}

void NeuronPropertiesWidget::disable(){
    if(m_model_parameters){
        ui->modelParameters->layout()->removeWidget(m_model_parameters);
        delete m_model_parameters;
        m_model_parameters = 0;
    }
    ui->type->setText("");
    ui->type->setEnabled(false);
    ui->currentSpinBox->setEnabled(false);
    ui->add_potential_button->setEnabled(false);
    ui->set_potential_button->setEnabled(false);
}

void NeuronPropertiesWidget::enable(){
    ui->type->setEnabled(true);
    ui->currentSpinBox->setEnabled(true);
    ui->add_potential_button->setEnabled(true);
    ui->set_potential_button->setEnabled(true);
}
