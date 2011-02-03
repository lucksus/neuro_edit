#include "neuron_properties_widget.h"
#include "ui_neuron_properties_widget.h"
#include "simulationobject.h"
#include "izhikevich.h"
#include "izhikevich_properties_widget.h"
#include <boost/foreach.hpp>
#include <assert.h>

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
    if(objects.empty()){
        return;
    }

    std::string type;
    BOOST_FOREACH(SimulationObject* o, objects){
        std::string new_type;
        if(dynamic_cast<Izhikevich*>(o))
            new_type = "Izhikevich";
        else
            new_type = "unknown";

        if(type == "") type = new_type;
        else if(type != new_type) return;
    }
    //all objects of same type
    ui->type->setText(type.c_str());
    enable();

    if(type == "Izhikevich"){
        std::set<Izhikevich*> neurons;
        BOOST_FOREACH(SimulationObject* o, objects){
            Izhikevich* n = dynamic_cast<Izhikevich*>(o);
            assert(n);
            neurons.insert(n);
        }

        if(neurons.size() == 1)
            m_model_parameters = new IzhikevichPropertiesWidget(*neurons.begin());
        else
            m_model_parameters = new IzhikevichPropertiesWidget(neurons);
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
