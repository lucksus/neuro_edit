#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"
#include "izhikevich.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_glscene(this)
{
    ui->setupUi(this);
    m_network = new Network;
    m_glscene.set_network(m_network);
    setCentralWidget(&m_glscene);
    m_glscene.setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSingle_Neuron_triggered(bool){
    m_network->add_object(new Izhikevich(Point(),0.02, 0.2, -65, 8));
    m_glscene.updateGL();
}

