#ifndef NEURON_MEMBRANE_PLOT_WIDGET_H
#define NEURON_MEMBRANE_PLOT_WIDGET_H

#include <QGLWidget>
#include <vector>

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

protected:
     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();


private:
    Neuron* m_neuron;

    void setup_projection_matrix();
    void paint_axis();

    std::vector< std::pair<double,double> > m_last_values;
};

class Neuron;
class NeuronMembranePlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NeuronMembranePlotWidget(QWidget *parent = 0);
    ~NeuronMembranePlotWidget();

    void set_neuron(Neuron*);

private slots:
    void on_last_milliseconds_changed(double value);

private:
    Ui::NeuronMembranePlotWidget *ui;
    GLMembranePlot m_plot;
};

#endif // NEURON_MEMBRANE_PLOT_WIDGET_H
