#ifndef APPLICATION_H
#define APPLICATION_H
#include "mainwindow.h"
#include "simulation.h"
#include "network.h"
#include <QTimer>
#include <QSplashScreen>
#include "scriptengine.h"

class Application : public QObject
{
Q_OBJECT
public:
    static Application& instance();
    void init();
    void destroy();
    void show_main_window();

    void close_simulation();
    void create_new_simulation();
    void load_simulation(std::string filename);
    void save_simulation(std::string filename);

    Simulation* simulation();

    QWidget* main_window();

signals:
    void refresh();
    void new_simulation(Simulation*);

private slots:
    void refresh_timeout();
    void hide_splash();

private:
    Application();
    QSplashScreen m_splash;
    MainWindow* m_main_window;
    Simulation* m_simulation;

    QTimer m_timer;

};

#endif // APPLICATION_H
