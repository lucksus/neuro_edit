#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "glscene.h"
#include "simulation.h"
#include "neuron_properties_widget.h"
#include "simulation_settings_widget.h"
#include "neuron_membrane_plot_widget.h"

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

protected:
    void closeEvent(QCloseEvent *event);

private slots:
     void on_actionSingle_Neuron_triggered(bool);
     void on_actionStart_Simulation_triggered(bool);
     void on_actionPause_Simulation_triggered(bool);
     void on_actionQuit_triggered(bool);

     void simulation_started();
     void simulation_stopped();

     void simulation_time_passed(double);

private:
    Ui::MainWindow *ui;
    GLScene m_glscene;
    Network* m_network;
    //NeuronPlotWidget m_neuron_potential_plot;
    NeuronPropertiesWidget m_neuron_properties;
    NeuronMembranePlotWidget m_neuron_membrane_potential_widget;
    Simulation m_sim;
    SimulationSettingsWidget m_sim_settings_widget;
    QTimer m_refresh_timer;
};

#endif // MAINWINDOW_H
