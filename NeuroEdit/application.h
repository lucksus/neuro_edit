#ifndef APPLICATION_H
#define APPLICATION_H
#include "mainwindow.h"
#include "simulation.h"
#include "network.h"
#include <QTimer>

class Application : public QObject
{
Q_OBJECT
public:
    static Application& instance();
    void init();
    void destroy();
    void show_main_window();

    Simulation* simulation();

signals:
    void refresh();

private slots:
    void refresh_timeout();

private:
    Application();
    MainWindow* m_main_window;
    Simulation* m_simulation;
    Network* m_network;

    QTimer m_timer;

};

#endif // APPLICATION_H
