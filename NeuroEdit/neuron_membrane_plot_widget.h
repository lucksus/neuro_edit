#ifndef NEURON_MEMBRANE_PLOT_WIDGET_H
#define NEURON_MEMBRANE_PLOT_WIDGET_H

#include <QGLWidget>
#include <deque>
#include "glplotwidget2d.h"

namespace Ui {
    class NeuronMembranePlotWidget;
}

class Neuron;
class Simulation;
class GLMembranePlot : public GLPlotWidget2d{
Q_OBJECT
public:
    GLMembranePlot(QWidget* parent = 0);

public slots:
    void set_neuron(Neuron*);
    void set_time_intervall(double milliseconds);
    void new_simulation(Simulation*);

protected:
    virtual list<GLPlotWidget2d::Point2DWithAlpha> data();

private slots:
    void simulation_started();

private:
    Neuron* m_neuron;
    double m_time_interval;
    double m_time;
    void update_values();

    std::deque< std::pair<double,double> > m_last_values;

    void setup();
};

class Neuron;
class NeuronMembranePlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NeuronMembranePlotWidget(QWidget *parent = 0);
    ~NeuronMembranePlotWidget();

public slots:
    void set_neuron(Neuron*);


private:
    Ui::NeuronMembranePlotWidget *ui;
    GLMembranePlot m_plot;
};

#endif // NEURON_MEMBRANE_PLOT_WIDGET_H
