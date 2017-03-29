#include "mlpvisualization.h"
#include "ui_mlpvisualization.h"
#include "multilayerperceptron.h"
#include <QGraphicsTextItem>
#include <QVector2D>

MLPVisualization::MLPVisualization(MultiLayerPerceptron *mlp) :
    QWidget(0),
    ui(new Ui::MLPVisualization),
    m_mlp(mlp),
    metrics(QFontMetrics(QFont()))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&m_scene);
    build_graphic_items();
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    ui->mlp_name_label->setText(mlp->objectName());
    connect(m_mlp, SIGNAL(changed()), this, SLOT(update()));
}

MLPVisualization::~MLPVisualization()
{
    delete ui;
}


void MLPVisualization::build_graphic_items(){
    qreal y = 0;
    m_items.resize(m_mlp->layers());
    for(unsigned int layer=0;layer<m_mlp->layers();layer++){
        const qreal layer_length = m_mlp->units_in_layer(layer)*NEURON_SIZE + (m_mlp->units_in_layer(layer)-1)*NEURON_MARGIN;
        const qreal left = -layer_length/2;
        qreal x = left;
        m_items[layer].ellipses.resize(m_mlp->units_in_layer(layer));
        m_items[layer].membrane_potentials.resize(m_mlp->units_in_layer(layer));
        m_items[layer].errors.resize(m_mlp->units_in_layer(layer));
        for(unsigned int neuron=0;neuron < m_mlp->units_in_layer(layer); neuron++){
            m_items[layer].ellipses[neuron] = m_scene.addEllipse(x,y, NEURON_SIZE, NEURON_SIZE);
            m_items[layer].membrane_potentials[neuron] = create_membrane_potential_text(m_items[layer].ellipses[neuron], layer, neuron);
            m_items[layer].errors[neuron] = create_error_text(m_items[layer].ellipses[neuron], layer, neuron);
            x += NEURON_SIZE + NEURON_MARGIN;
        }
        y -= LAYER_MARGIN;
    }

    for(unsigned int layer=0;layer<m_mlp->layers()-1;layer++){
        m_items[layer].weights.resize(m_mlp->units_in_layer(layer));
        m_items[layer].lines.resize(m_mlp->units_in_layer(layer));
        for(unsigned int i=0;i < m_mlp->units_in_layer(layer); i++){
            m_items[layer].weights[i].resize(m_mlp->units_in_layer(layer+1));
            m_items[layer].lines[i].resize(m_mlp->units_in_layer(layer+1));
            QGraphicsEllipseItem* start = m_items[layer].ellipses[i];
            for(unsigned int j=0;j < m_mlp->units_in_layer(layer+1); j++){
                QGraphicsEllipseItem* end = m_items[layer+1].ellipses[j];
                QVector2D vec = QVector2D(end->rect().center() - start->rect().center());
                vec.normalize();
                QPointF start_point = start->rect().center() + QVector2D(vec*NEURON_SIZE/2).toPointF();
                QPointF end_point = end->rect().center() - QVector2D(vec*NEURON_SIZE/2).toPointF();
                QGraphicsLineItem* line = m_scene.addLine(start_point.x(), start_point.y(), end_point.x(), end_point.y());
                m_items[layer].weights[i][j] = create_weight_text(line, m_mlp->m_weights[layer][i][j]);
                m_items[layer].lines[i][j] = line;
            }
        }
    }
}

QGraphicsTextItem* MLPVisualization::create_membrane_potential_text(QGraphicsEllipseItem* ellipse, unsigned int layer, unsigned int neuron){
    QString membrane_potential(QString("%1").arg(m_mlp->m_membrane_potentials[layer][neuron]));
    QGraphicsTextItem *item= m_scene.addText(membrane_potential);
    qreal x = ellipse->rect().center().x();
    qreal y = ellipse->rect().center().y();
    item->setPos(x-metrics.width(membrane_potential)/2,y-metrics.height());
    return item;
}

QGraphicsTextItem* MLPVisualization::create_error_text(QGraphicsEllipseItem* ellipse, unsigned int layer, unsigned int neuron){
    QString error(QString("%1").arg(m_mlp->m_errors[layer][neuron]));
    QGraphicsTextItem *item= m_scene.addText(error);
    qreal x = ellipse->rect().center().x();
    qreal y = ellipse->rect().center().y();
    item->setPos(x-metrics.width(error)/2,y+NEURON_SIZE/2);
    item->setDefaultTextColor(QColor(255,0,0));
    return item;
}

QGraphicsTextItem* MLPVisualization::create_weight_text(QGraphicsLineItem* line, double weight){
    QString w = QString::number(weight,'f',3);
    QGraphicsTextItem *item= m_scene.addText(w);

    qreal angle = line->line().angle();
    if(angle > 90) item->rotate(180-angle);
    else item->rotate(-angle);

    item->setPos(line->line().pointAt(0.5));
    item->setDefaultTextColor(QColor(30,0,0));

    return item;
}

void MLPVisualization::update(){
    for(unsigned int layer=0;layer<m_mlp->m_number_of_units_per_layer.size();layer++){
        for(unsigned int neuron=0;neuron<m_mlp->m_number_of_units_per_layer[layer];neuron++){
            delete m_items[layer].membrane_potentials[neuron];
            m_items[layer].membrane_potentials[neuron] = create_membrane_potential_text(m_items[layer].ellipses[neuron], layer, neuron);
            delete m_items[layer].errors[neuron];
            m_items[layer].errors[neuron] = create_error_text(m_items[layer].ellipses[neuron], layer, neuron);
            if(layer < m_mlp->m_number_of_units_per_layer.size()-1)
                for(unsigned int weight=0;weight<m_mlp->m_number_of_units_per_layer[layer+1];weight++){
                    delete m_items[layer].weights[neuron][weight];
                    m_items[layer].weights[neuron][weight] = create_weight_text(m_items[layer].lines[neuron][weight], m_mlp->m_weights[layer][neuron][weight]);
                }
        }
    }
}
