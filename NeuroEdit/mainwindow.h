#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "glscene.h"
#include "simulation.h"
#include "simulation_settings_widget.h"
#include "neuron_membrane_plot_widget.h"
#include "izhikevich_systemstate_plotwidget.h"
#include "propertybrowser.h"
#include "scriptswindow.h"
#include "logwindow.h"
#include "settingsdialog.h"

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

    void readSettings();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
     void on_actionSingle_Neuron_triggered(bool);
     void on_actionAxon_Node_triggered(bool);
     void on_actionDendrite_Node_triggered(bool);
     void on_actionSynapse_triggered(bool);
     void on_actionCurrent_Inducer_triggered(bool);
     void on_actionSamples_triggered(bool);
     void on_actionLinear_Discriminator_triggered(bool);
     void on_actionGroup_triggered(bool);

     void on_actionStart_Simulation_triggered(bool);
     void on_actionPause_Simulation_triggered(bool);

     void on_actionNew_triggered(bool);
     void on_actionSave_triggered(bool);
     void on_actionSave_as_triggered(bool);
     void on_actionLoad_triggered(bool);
     void on_actionClose_triggered(bool);
     void on_actionExport_Simulation_to_XML_triggered(bool);
     void on_actionImport_Simulation_from_XML_triggered(bool);
     void on_actionSettings_triggered(bool);
     void on_actionQuit_triggered(bool);

     void on_actionCut_triggered(bool);
     void on_actionCopy_triggered(bool);
     void on_actionPaste_triggered(bool);
     void on_actionRemove_triggered(bool);

     void on_actionConnect_triggered(bool);

     void on_actionAbout_triggered(bool);

     void simulation_started();
     void simulation_stopped();

     void simulation_time_passed(double);

     void objects_selected(std::set<SimulationObject*>);
     void right_click_on_object_during_multiselection(QPoint);
     void simulation_changed(Simulation*);

     QStringList recentlyUsedFiles() const;
     void setRecentlyUsedFiles(QStringList files);
     void openRecentlyUsedFile();
     void populateRecentlyUsedMenu();
     void addFileToRecentlyUsed(QString filePath);
     void clearRecentlyUsedFiles();
     void populateAddToGroupMenu();
     void add_selected_to_group();
     void on_menuSelection_aboutToShow();

private:
    Ui::MainWindow *ui;
    GLScene* m_glscene;
    Network* m_network;

    NeuronMembranePlotWidget m_neuron_membrane_potential_widget;
    IzhikevichSystemStatePlotWidget m_izhikevich_system_plot_widget;
    Simulation* m_sim;
    SimulationSettingsWidget m_sim_settings_widget;
    PropertyBrowser m_property_browser;
    ScriptsWindow m_scripts_window;
    LogWindow m_log_window;
    SettingsDialog m_settings_dialog;

    std::set<QDockWidget*> m_dock_widgets;

    std::string m_clipboard;
    std::set<SimulationObject*> selected_objects_cloned_and_self_centered();

    void init_glscene();
    void set_insert_actions_enabled(bool enabled);
};

#endif // MAINWINDOW_H
