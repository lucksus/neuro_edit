#ifndef MLPVISUALIZATION_H
#define MLPVISUALIZATION_H

#include <QWidget>
#include <QGraphicsScene>
//#include <QGraphicsEllipseItem>
//#include <QGraphicsTextItem>

#define NEURON_SIZE 30
#define NEURON_MARGIN 50
#define LAYER_MARGIN 180

namespace Ui {
class MLPVisualization;
}


class MultiLayerPerceptron;

struct Layer{
    QVector<QGraphicsEllipseItem*> ellipses;
    QVector<QGraphicsTextItem*> membrane_potentials;
    QVector<QGraphicsTextItem*> errors;
    QVector< QVector<QGraphicsTextItem*> > weights;
    QVector< QVector<QGraphicsLineItem*> > lines;
};

class MLPVisualization : public QWidget
{
    Q_OBJECT
    
public:
    explicit MLPVisualization(MultiLayerPerceptron*);
    ~MLPVisualization();
    
public slots:
    void update();

private:
    Ui::MLPVisualization *ui;
    MultiLayerPerceptron* m_mlp;
    QGraphicsScene m_scene;
    QVector<Layer> m_items;

    QFontMetrics metrics;

    void build_graphic_items();

    QGraphicsTextItem* create_membrane_potential_text(QGraphicsEllipseItem* ellipse, unsigned int layer, unsigned int neuron);
    QGraphicsTextItem* create_error_text(QGraphicsEllipseItem* ellipse, unsigned int layer, unsigned int neuron);
    QGraphicsTextItem* create_weight_text(QGraphicsLineItem* ellipse, double weight);

};

#endif // MLPVISUALIZATION_H
