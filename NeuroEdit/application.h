#ifndef APPLICATION_H
#define APPLICATION_H
#include "mainwindow.h"
#include "simulation.h"
#include "network.h"
#include <QTimer>
#include <QSplashScreen>

class Application : public QObject
{
Q_OBJECT
public:
    static Application& instance();
    void init();
    void destroy();
    void show_main_window();

    void close_network();
    void create_empty_network();
    void load_network(std::string filename);
    void save_network(std::string filename);

    Network* network();
    Simulation* simulation();

    QWidget* main_window();

signals:
    void refresh();
    void new_network(Network*);

private slots:
    void refresh_timeout();
    void hide_splash();

private:
    Application();
    QSplashScreen m_splash;
    MainWindow* m_main_window;
    Simulation* m_simulation;
    Network* m_network;

    QTimer m_timer;

};

#endif // APPLICATION_H
