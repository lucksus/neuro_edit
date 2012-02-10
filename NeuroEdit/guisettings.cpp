#include "guisettings.h"
#include <QtCore/QSettings>

GuiSettings::GuiSettings()
{
    load();
}


GuiSettings& GuiSettings::instance(){
    static GuiSettings s;
    return s;
}

void GuiSettings::load(){
    QSettings settings;
    graphics.neuron_detail = settings.value("graphics/neurons_detail",1).toInt();
    graphics.axon_detail = settings.value("graphics/axons_detail",1).toInt();
    graphics.synapse_detail = settings.value("graphics/synapses_detail",1).toInt();
}

void GuiSettings::save(){
    QSettings settings;
    settings.setValue("graphics/neurons_detail", graphics.neuron_detail);
    settings.setValue("graphics/axons_detail", graphics.axon_detail);
    settings.setValue("graphics/synapses_detail", graphics.synapse_detail);
}


int GuiSettings::hash(){
    return graphics.neuron_detail + 10*graphics.axon_detail + 100*graphics.synapse_detail;
}
