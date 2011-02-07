#include "simulation_settings_widget.h"
#include "ui_simulation_settings_widget.h"
#include "simulation.h"
#include <assert.h>

SimulationSettingsWidget::SimulationSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationSettingsWidget)
{
    ui->setupUi(this);
    ui->run_button->setText("run");
}

SimulationSettingsWidget::~SimulationSettingsWidget()
{
    delete ui;
}


void SimulationSettingsWidget::set_simulation(Simulation* sim){
    m_simulation = sim;
    ui->real_step->setEnabled(m_simulation);
    ui->simulation_step->setEnabled(m_simulation);
    ui->run_button->setEnabled(m_simulation);

    if(m_simulation){
        ui->simulation_step->setValue(m_simulation->simulation_step());
        ui->real_step->setValue(m_simulation->real_step());
    }
}

void SimulationSettingsWidget::simulation_started(){
    ui->run_button->setText("stop");
}

void SimulationSettingsWidget::simulation_stopped(){
    ui->run_button->setText("run");
}

void SimulationSettingsWidget::on_run_button_clicked(){
    assert(m_simulation);
    if(ui->run_button->text() == "run")
        m_simulation->start();
    else
        m_simulation->request_stop();
}


void SimulationSettingsWidget::on_simulation_step_valueChanged(double value){
    assert(m_simulation);
    m_simulation->set_simulation_step(value);
}

void SimulationSettingsWidget::on_real_step_valueChanged(double value){
    assert(m_simulation);
    m_simulation->set_real_step(value);
}
