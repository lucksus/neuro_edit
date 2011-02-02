#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glscene.h"

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

private:
    Ui::MainWindow *ui;
    GLScene m_glscene;
    Network* m_network;
};

#endif // MAINWINDOW_H
