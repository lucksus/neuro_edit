#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glscene.h"
#include "simulation.h"
#include "neuron_properties_widget.h"

namespace Ui {
    class MainWindow;
}
class Network;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
     void on_actionSingle_Neuron_triggered(bool);
     void on_actionStart_Simulation_triggered(bool);
     void on_actionPause_Simulation_triggered(bool);

     void simulation_started();
     void simulation_stopped();
private:
    Ui::MainWindow *ui;
    GLScene m_glscene;
    Network* m_network;
    //NeuronPlotWidget m_neuron_potential_plot;
    NeuronPropertiesWidget m_neuron_properties;
    Simulation m_sim;
};

#endif // MAINWINDOW_H
