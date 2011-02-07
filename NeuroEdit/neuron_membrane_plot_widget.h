#ifndef NEURON_MEMBRANE_PLOT_WIDGET_H
#define NEURON_MEMBRANE_PLOT_WIDGET_H

#include <QGLWidget>
#include <deque>

namespace Ui {
    class NeuronMembranePlotWidget;
}

class Neuron;

class GLMembranePlot : public QGLWidget{
Q_OBJECT
public:
    GLMembranePlot(QWidget* parent = 0);

public slots:
    void milliseconds_passed(double milliseconds);

    void set_neuron(Neuron*);
    void set_time_intervall(double milliseconds);

protected:
     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();


private:
    Neuron* m_neuron;
    double m_time_interval;

    void setup_projection_matrix();
    void paint_axis();
    void paint_values();

    std::deque< std::pair<double,double> > m_last_values;
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

private slots:
    void milliseconds_passed(double milliseconds);

private:
    Ui::NeuronMembranePlotWidget *ui;
    GLMembranePlot m_plot;
};

#endif // NEURON_MEMBRANE_PLOT_WIDGET_H
