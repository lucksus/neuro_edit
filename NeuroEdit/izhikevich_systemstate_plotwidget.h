#ifndef IZHIKEVICH_SYSTEMSTATE_PLOTWIDGET_H
#define IZHIKEVICH_SYSTEMSTATE_PLOTWIDGET_H
#include "glplotwidget2d.h"
#include <QTimer>

class Izhikevich;
class Neuron;
class IzhikevichSystemStatePlotWidget : public GLPlotWidget2d
{
Q_OBJECT
public:
    IzhikevichSystemStatePlotWidget(QWidget* parent);

public slots:
    void set_neuron(Neuron*);

protected:
    virtual list<Point2DWithAlpha> data();

private:
    Izhikevich* m_neuron;
    QTimer m_timer;

    list<GLPlotWidget2d::Point2DWithAlpha> m_last_values;
};

#endif // IZHIKEVICH_SYSTEMSTATE_PLOTWIDGET_H
