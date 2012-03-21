#include "guiuserinteractionadapter.h"
#include <QtGui/QFileDialog>
#include "Visualizer.h"
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include "multilayerperceptron.h"
#include "mlpvisualization.h"
#include <QtCore/QThread>
#include "lsmreadoutneuron.h"
#include "readoutvisualization.h"

GuiUserInteractionAdapter::GuiUserInteractionAdapter(){
    m_context = Visualizer::getInstance().createContext2D();
    m_context->setDescription("Samples");
    m_context->addAxis("Time", "ms");
    m_context->addAxis("Sample values", "V/s");
    m_context->setShowTime(true);
    m_context->setTimeColor(1.,1.,0.,0.85);
}

std::string GuiUserInteractionAdapter::get_save_filepath(std::string file_type, std::string source, std::string){
    return QFileDialog::getSaveFileName(0, source.c_str(), QString(), file_type.c_str()).toStdString();
}

std::string GuiUserInteractionAdapter::get_load_filepath(std::string file_type, std::string source, std::string){
    return QFileDialog::getOpenFileName(0, source.c_str(), QString(), file_type.c_str()).toStdString();
}

void GuiUserInteractionAdapter::display_samples(const std::vector<sample>& samples, string name){

    VisualizerData2D* data = 0;
    if(m_visualizer_datas.count(name)) data = m_visualizer_datas[name];
    else data = new VisualizerData2D;

    data->setName(name);
    data->setUnit("V/s");
    data->setShowReal();
    data->resizeData(samples.size());
    for(unsigned int i=0;i<samples.size();i++)
        data->writeData(i, samples[i].time, samples[i].value);

    if(!m_visualizer_datas.count(name)){
        m_visualizer_datas[name] = data;
        m_context->addData(data);
    }
    Visualizer::getInstance().showWindowFor(m_context);
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


vector<int> GuiUserInteractionAdapter::get_integer_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<int,int> > limits ){
    QDialog dialog;

    dialog.setWindowTitle(QString("%1 requires input").arg(source.c_str()));
    dialog.setLayout(new QVBoxLayout);
    dialog.layout()->addWidget(new QLabel(porpuse.c_str()));

    vector<QSpinBox*> spin_boxes;

    for(unsigned int i=0;i<value_descriptions.size();i++){
        QFrame* frame = new QFrame(&dialog);
        frame->setLayout(new QHBoxLayout);
        QLabel* label = new QLabel(&dialog);
        label->setText(QString("%1: ").arg(value_descriptions[i].c_str()));
        frame->layout()->addWidget(label);

        QSpinBox* spinBox = new QSpinBox(&dialog);
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

    vector<int> return_value;
    if(dialog.exec()){
        return_value.resize(value_descriptions.size());
        for(unsigned int i=0;i<value_descriptions.size();i++){
            return_value[i] = spin_boxes[i]->value();
        }
    }

    return return_value;
}

void GuiUserInteractionAdapter::confirm_message(std::string message){
    QMessageBox::information(0,"Neuro Edit Information", message.c_str());
}

void GuiUserInteractionAdapter::display_image(QImage *image){
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap::fromImage(*image));
    label->show();
}

void GuiUserInteractionAdapter::display_mlp(MultiLayerPerceptron *mlp){
    if(QThread::currentThread() != thread()){
        QMetaObject::invokeMethod(this, "display_mlp", Qt::QueuedConnection, Q_ARG(MultiLayerPerceptron*, mlp));
        return;
    }
    MLPVisualization* mlpviz = new MLPVisualization(mlp);
    mlpviz->show();
}

void GuiUserInteractionAdapter::display_read_out_weights(LSMReadOutNeuron* read_out_neuron){
    if(QThread::currentThread() != thread()){
        QMetaObject::invokeMethod(this, "display_read_out_weights", Qt::QueuedConnection, Q_ARG(LSMReadOutNeuron*, read_out_neuron));
        return;
    }

    static map<LSMReadOutNeuron*,ReadOutVisualization*> list_widgets;
    if(list_widgets.count(read_out_neuron)){
        list_widgets[read_out_neuron]->show();
        return;
    }

    ReadOutVisualization* viz = new ReadOutVisualization(read_out_neuron);
    viz->show();
    list_widgets[read_out_neuron] = viz;
}
