#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "glscene.h"
#include "simulation.h"
#include "neuron_properties_widget.h"
#include "simulation_settings_widget.h"
#include "neuron_membrane_plot_widget.h"
#include "izhikevich_systemstate_plotwidget.h"
#include "neuron_manipulator_widget.h"
#include "propertybrowser.h"

namespace Ui {
    class MainWindow;
}
class Network;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Simulation* sim, QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
     void on_actionSingle_Neuron_triggered(bool);
     void on_actionAxon_Node_triggered(bool);
     void on_actionDendrite_Node_triggered(bool);
     void on_actionSynapse_triggered(bool);

     void on_actionStart_Simulation_triggered(bool);
     void on_actionPause_Simulation_triggered(bool);

     void on_actionSave_triggered(bool);
     void on_actionLoad_triggered(bool);
     void on_actionQuit_triggered(bool);

     void on_actionCut_triggered(bool);
     void on_actionCopy_triggered(bool);
     void on_actionPaste_triggered(bool);

     void on_actionConnect_triggered(bool);

     void on_actionAbout_triggered(bool);

     void simulation_started();
     void simulation_stopped();

     void simulation_time_passed(double);

     void objects_selected(std::set<SimulationObject*>);

private:
    Ui::MainWindow *ui;
    GLScene m_glscene;
    Network* m_network;
    //NeuronPlotWidget m_neuron_potential_plot;
    NeuronPropertiesWidget m_neuron_properties;
    NeuronMembranePlotWidget m_neuron_membrane_potential_widget;
    NeuronManipulatorWidget m_neuron_manipulator_widget;
    IzhikevichSystemStatePlotWidget m_izhikevich_system_plot_widget;
    Simulation* m_sim;
    SimulationSettingsWidget m_sim_settings_widget;
    PropertyBrowser m_property_browser;

    std::string m_clipboard;
    std::set<SimulationObject*> selected_objects_cloned_and_self_centered();
};

#endif // MAINWINDOW_H
