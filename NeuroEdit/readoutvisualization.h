#ifndef READOUTVISUALIZATION_H
#define READOUTVISUALIZATION_H

#include <QWidget>
#include <QListWidget>
#include <QTimer>

class LSMReadOutNeuron;

namespace Ui {
class ReadOutVisualization;
}

class ReadOutVisualization : public QWidget
{
    Q_OBJECT
    
public:
    explicit ReadOutVisualization(LSMReadOutNeuron* neuron, QWidget *parent = 0);
    ~ReadOutVisualization();

public slots:
    void update();

private:
    LSMReadOutNeuron* m_read_out_neuron;
    std::vector<QListWidgetItem*> m_items;
    QTimer m_timer;
    Ui::ReadOutVisualization *ui;
};

#endif // READOUTVISUALIZATION_H
