#include "izhikevich_properties_widget.h"
#include "ui_izhikevich_properties_widget.h"
#include "izhikevich.h"

IzhikevichPropertiesWidget::IzhikevichPropertiesWidget(Izhikevich* neuron, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IzhikevichPropertiesWidget),
    m_neuron(neuron)
{
    ui->setupUi(this);
    ui->a->setValue(m_neuron->get_a());
    ui->b->setValue(m_neuron->get_b());
    ui->c->setValue(m_neuron->get_c());
    ui->d->setValue(m_neuron->get_d());

}

IzhikevichPropertiesWidget::~IzhikevichPropertiesWidget()
{
    delete ui;
}

void IzhikevichPropertiesWidget::on_set_button_clicked(){
    m_neuron->set_a(ui->a->value());
    m_neuron->set_b(ui->b->value());
    m_neuron->set_c(ui->c->value());
    m_neuron->set_d(ui->d->value());
}

void IzhikevichPropertiesWidget::on_comboBox_activated(int type){
    switch(type){
    case 0:
        //rs
        ui->a->setValue(0.02);
        ui->b->setValue(0.2);
        ui->c->setValue(-65);
        ui->d->setValue(8);
        break;
    case 1:
        //ib
        ui->a->setValue(0.02);
        ui->b->setValue(0.2);
        ui->c->setValue(-55);
        ui->d->setValue(4);
        break;
    case 2:
        //ch
        ui->a->setValue(0.02);
        ui->b->setValue(0.2);
        ui->c->setValue(-50);
        ui->d->setValue(2);
        break;
    case 3:
        //fs
        ui->a->setValue(0.01);
        ui->b->setValue(0.2);
        ui->c->setValue(-65);
        ui->d->setValue(2);
        break;
    case 4:
        //tc
        ui->a->setValue(0.02);
        ui->b->setValue(0.25);
        ui->c->setValue(-65);
        ui->d->setValue(0.05);
        break;
    case 5:
        //rz
        ui->a->setValue(0.1);
        ui->b->setValue(0.26);
        ui->c->setValue(-65);
        ui->d->setValue(2);
        break;
    case 6:
        //lts
        ui->a->setValue(0.02);
        ui->b->setValue(0.25);
        ui->c->setValue(-65);
        ui->d->setValue(2);
        break;
    }
}
