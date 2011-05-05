#ifndef APPLICATION_H
#define APPLICATION_H
#include "mainwindow.h"
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

    QWidget* main_window();

signals:
    void refresh();

private slots:
    void refresh_timeout();
    void hide_splash();

private:
    Application();
    QSplashScreen m_splash;
    MainWindow* m_main_window;

    QTimer m_timer;

};

#endif // APPLICATION_H
