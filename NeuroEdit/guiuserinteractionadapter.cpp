#include "guiuserinteractionadapter.h"
#include <QtGui/QFileDialog>
#include "Visualizer.h"
#include "VisualizerContext2D.h"
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QPushButton>

std::string GuiUserInteractionAdapter::get_save_filepath(std::string file_type, std::string source, std::string){
    return QFileDialog::getSaveFileName(0, source.c_str(), QString(), file_type.c_str()).toStdString();
}

std::string GuiUserInteractionAdapter::get_load_filepath(std::string file_type, std::string source, std::string){
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

vector<double> GuiUserInteractionAdapter::get_double_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<double,double> > limits = vector<pair<double,double> >()){
    QDialog dialog;

    dialog.setWindowTitle(QString("%1 requires input").arg(source.c_str()));
    dialog.setLayout(new QVBoxLayout);
    dialog.layout()->addWidget(new QLabel(porpuse.c_str()));

    vector<QDoubleSpinBox*> spin_boxes;

    for(unsigned int i=0;i<value_descriptions.size();i++){
        QFrame* frame = new QFrame(&dialog);
        frame->setLayout(new QHBoxLayout);
        QLabel* label = new QLabel(&dialog);
        label->setText(QString("%1: ").arg(value_descriptions[i].c_str()));
        frame->layout()->addWidget(label);

        QDoubleSpinBox* spinBox = new QDoubleSpinBox(&dialog);
        spinBox->setDecimals(5);
        if(limits.size() == value_descriptions.size()){
            spinBox->setMinimum(limits[i].first);
            spinBox->setMaximum(limits[i].second);
        }

        frame->layout()->addWidget(spinBox);
        spin_boxes.push_back(spinBox);
        dialog.layout()->addWidget(frame);

    }

    QPushButton* button = new QPushButton;
    button->setText("Ok");
    QObject::connect(button,SIGNAL(pressed()),&dialog,SLOT(accept()));
    dialog.layout()->addWidget(button);

    vector<double> return_value;
    if(dialog.exec()){
        return_value.resize(value_descriptions.size());
        for(unsigned int i=0;i<value_descriptions.size();i++){
            return_value[i] = spin_boxes[i]->value();
        }
    }

    return return_value;

}
