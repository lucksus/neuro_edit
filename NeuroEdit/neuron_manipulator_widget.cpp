#include "neuron_manipulator_widget.h"
#include "ui_neuron_manipulator_widget.h"
#include <boost/foreach.hpp>
#include "application.h"

NeuronManipulatorWidget::NeuronManipulatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeuronManipulatorWidget)
{
    ui->setupUi(this);
}

NeuronManipulatorWidget::~NeuronManipulatorWidget()
{
    delete ui;
}

void NeuronManipulatorWidget::set_dendritic_nodes(std::set<SimulationObject*> objects){
    m_dendritic_nodes.clear();
    BOOST_FOREACH(SimulationObject* o, objects){
        DendriticNode* n = dynamic_cast<DendriticNode*>(o);
        if(n) m_dendritic_nodes.insert(n);
    }

    create_current_inducers();
}

void NeuronManipulatorWidget::create_current_inducers(){
    delete_current_inducers();
    BOOST_FOREACH(DendriticNode* n, m_dendritic_nodes){
            CurrentInducer* ci = new CurrentInducer(n);
            m_current_inducers.insert(ci);
            Application::instance().simulation()->network()->add_object(ci);
    }
}

void NeuronManipulatorWidget::delete_current_inducers(){
    BOOST_FOREACH(CurrentInducer* ci, m_current_inducers){
        Application::instance().simulation()->network()->delete_object(ci);
    }
    m_current_inducers.clear();
}

void NeuronManipulatorWidget::activate_current_inducers(){
    BOOST_FOREACH(CurrentInducer* ci, m_current_inducers){
        ci->set_current(ui->currentSpinBox->value());
        ci->set_active(true);
    }
}

void NeuronManipulatorWidget::deactivate_current_inducers(){
    BOOST_FOREACH(CurrentInducer* ci, m_current_inducers){
        ci->set_active(false);
    }
}

void NeuronManipulatorWidget::on_apply_potential_button_pressed(){
    activate_current_inducers();
}

void NeuronManipulatorWidget::on_apply_potential_button_released(){
    deactivate_current_inducers();
}
