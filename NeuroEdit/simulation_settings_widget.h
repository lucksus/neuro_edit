#ifndef SIMULATION_SETTINGS_WIDGET_H
#define SIMULATION_SETTINGS_WIDGET_H

#include <QWidget>

namespace Ui {
    class SimulationSettingsWidget;
}

class Simulation;
class SimulationSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimulationSettingsWidget(QWidget *parent = 0);
    ~SimulationSettingsWidget();

    void set_simulation(Simulation*);

public slots:
    void simulation_started();
    void simulation_stopped();

private slots:
    void on_run_button_clicked();
    void on_simulation_step_changed(double value);
    void on_real_step_changed(double value);


private:
    Ui::SimulationSettingsWidget *ui;
    Simulation* m_simulation;
};

#endif // SIMULATION_SETTINGS_WIDGET_H
