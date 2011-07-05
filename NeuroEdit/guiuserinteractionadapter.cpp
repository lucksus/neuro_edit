#include "guiuserinteractionadapter.h"
#include <QtGui/QFileDialog>
#include "Visualizer.h"
#include "VisualizerContext2D.h"

std::string GuiUserInteractionAdapter::get_save_filepath(std::string file_type, std::string source, std::string){
    return QFileDialog::getSaveFileName(0, source.c_str(), QString(), file_type.c_str()).toStdString();
}

std::string GuiUserInteractionAdapter::get_load_filepath(std::string file_type, std::string source, std::string porpuse){
    return QFileDialog::getOpenFileName(0, source.c_str(), QString(), file_type.c_str()).toStdString();
}

void GuiUserInteractionAdapter::display_samples(const std::vector<sample>& samples){
    VisualizerContext2D* context = Visualizer::getInstance().createContext2D();
    context->setDescription("Samples");
    context->addAxis("Time", "ms");
    context->addAxis("Sample values", "V/s");

    VisualizerData2D* data = new VisualizerData2D;
    data->setName("Sample values");
    data->setUnit("V/s");
    data->setShowReal();
    data->resizeData(samples.size());
    for(unsigned int i=0;i<samples.size();i++)
        data->writeData(i, samples[i].time, samples[i].value);

    context->addData(data);
    Visualizer::getInstance().showWindowFor(context);
}

