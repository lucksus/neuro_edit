#include "readoutvisualization.h"
#include "ui_readoutvisualization.h"
#include "lsmreadoutneuron.h"

ReadOutVisualization::ReadOutVisualization(LSMReadOutNeuron* neuron, QWidget *parent) :
    QWidget(parent), m_read_out_neuron(neuron), m_timer(this),

    ui(new Ui::ReadOutVisualization)
{
    ui->setupUi(this);
    ui->name->setText(neuron->objectName());
    ui->bias->setText(QString::number(neuron->bias()));
    ui->bias_weight->setText(QString::number(neuron->bias_weight()));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer.start(100);
}

ReadOutVisualization::~ReadOutVisualization()
{
    delete ui;
}

void ReadOutVisualization::update(){
    if(m_read_out_neuron->read_out_connections().size() != m_items.size()){
        for(unsigned int i=0;i<m_items.size();i++){
            delete m_items[i];
        }
        m_items.resize(m_read_out_neuron->read_out_connections().size());
        for(unsigned int i=0;i<m_items.size();i++){
            m_items[i] = new QListWidgetItem(ui->listWidget);
            m_items[i]->setText(QString("%1").arg(m_read_out_neuron->read_out_connections()[i].weight));
        }
    }else{
        for(unsigned int i=0;i<m_items.size();i++){
            m_items[i]->setText(QString("%1").arg(m_read_out_neuron->read_out_connections()[i].weight));
        }
    }
}
