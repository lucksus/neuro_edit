#include "izhikevich_properties_widget.h"
#include "ui_izhikevich_properties_widget.h"
#include "izhikevich.h"
#include <boost/foreach.hpp>

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

IzhikevichPropertiesWidget::IzhikevichPropertiesWidget(std::set<Izhikevich*> neurons, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IzhikevichPropertiesWidget),
    m_neuron(0),
    m_neurons(neurons)
{
    ui->setupUi(this);
    double a,b,c,d;
    bool a_different=false, b_different=false, c_different=false, d_different=false;
    Izhikevich* n = *neurons.begin();
    a = n->get_a();
    b = n->get_b();
    c = n->get_c();
    d = n->get_d();

    BOOST_FOREACH(n, neurons){
        if(a != n->get_a()) { a=n->get_a(); a_different=true; }
        if(b != n->get_b()) { b=n->get_b(); b_different=true; }
        if(c != n->get_c()) { c=n->get_c(); c_different=true; }
        if(d != n->get_d()) { d=n->get_d(); d_different=true; }
    }

    if(!a_different) ui->a->setValue(a);
    else ui->label_a->setText("a:*");
    if(!b_different) ui->b->setValue(b);
    else ui->label_b->setText("b:*");
    if(!c_different) ui->c->setValue(c);
    else ui->label_c->setText("c:*");
    if(!d_different) ui->d->setValue(d);
    else ui->label_d->setText("d:*");

    ui->set_button->setText("set all");
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
