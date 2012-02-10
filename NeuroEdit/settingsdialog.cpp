#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "guisettings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}


void SettingsDialog::showEvent ( QShowEvent * event ){
    GuiSettings::instance().load();
    ui->draw_neurons_comobBox->setCurrentIndex(GuiSettings::instance().graphics.neuron_detail);
    ui->draw_axons_comobBox->setCurrentIndex(GuiSettings::instance().graphics.axon_detail);
    ui->draw_synapses_comobBox->setCurrentIndex(GuiSettings::instance().graphics.synapse_detail);

    QDialog::showEvent(event);
}

void SettingsDialog::accept(){
    GuiSettings::instance().graphics.neuron_detail = ui->draw_neurons_comobBox->currentIndex();
    GuiSettings::instance().graphics.axon_detail = ui->draw_axons_comobBox->currentIndex();
    GuiSettings::instance().graphics.synapse_detail = ui->draw_synapses_comobBox->currentIndex();
    GuiSettings::instance().save();
    QDialog::accept();
}
