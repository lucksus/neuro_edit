#include "scriptswindow.h"
#include "ui_scriptswindow.h"
#include "network.h"
#include "simulation.h"
#include "application.h"
#include "controller.h"
#include <QtCore/QFile>
#include <QtGui/QFileDialog>

ScriptsWindow::ScriptsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptsWindow)
{
    ui->setupUi(this);
    delete ui->simulationScriptsList;
    ui->simulationScriptsList = new MyListView(this);
    static_cast<QBoxLayout*>(ui->tab->layout())->insertWidget(0,ui->simulationScriptsList);
    delete ui->networkScriptsList;
    ui->networkScriptsList = new MyListView(this);
    static_cast<QBoxLayout*>(ui->tab_2->layout())->insertWidget(0,ui->networkScriptsList);

    ui->simulationScriptsList->setModel(&m_simulation_scripts);
    ui->networkScriptsList->setModel(&m_network_scripts);
    ui->simulationScriptsList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->networkScriptsList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_syntax_highlighter = new ScriptSyntaxHighlighter(ui->textEdit);

    connect(ui->simulationScriptsList, SIGNAL(selection_changed(const QItemSelection&, const QItemSelection &)), this, SLOT(simulationScriptSelected(QItemSelection,QItemSelection)));
    connect(ui->networkScriptsList, SIGNAL(selection_changed(const QItemSelection&, const QItemSelection &)), this, SLOT(networkScriptSelected(QItemSelection,QItemSelection)));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(&m_simulation_scripts, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(simulationScriptsNameChanged(QModelIndex,QModelIndex)));
    connect(&m_network_scripts, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(networkScriptsNameChanged(QModelIndex,QModelIndex)));
    //connect(ui->simulationScriptsList, SIGNAL(activated(QModelIndex)), this, SLOT(simulationScriptSelected(QModelIndex)));
    connect(&Controller::instance(), SIGNAL(script_output(QString)), this, SLOT(script_output(QString)));
}

void ScriptsWindow::simulationScriptSelected(const QItemSelection & selected, const QItemSelection &){
    QModelIndex index = selected.indexes().first();
    if(QModelIndex() == index){
        ui->tabWidget_right->setEnabled(false);
        ui->textEdit->setText("");
        return;
    }
    ui->tabWidget_right->setEnabled(true);
    ui->textEdit->setEnabled(true);
    QString script_name = m_simulation_scripts.data(index, Qt::DisplayRole).toString();
    ui->textEdit->setText(Controller::instance().simulation()->script(script_name));
    m_current_script = script_name;
}

void ScriptsWindow::networkScriptSelected(const QItemSelection & selected, const QItemSelection &){
    QModelIndex index = selected.indexes().first();
    if(QModelIndex() == index){
        ui->tabWidget_right->setEnabled(false);
        ui->textEdit->setText("");
        return;
    }
    ui->tabWidget_right->setEnabled(true);
    ui->textEdit->setEnabled(true);
    QString script_name = m_network_scripts.data(index, Qt::DisplayRole).toString();
    ui->textEdit->setText(Controller::instance().simulation()->network()->script(script_name));
    m_current_script = script_name;
}

ScriptsWindow::~ScriptsWindow()
{
    delete ui;
}

void ScriptsWindow::read_name_lists(){
    m_simulation_scripts.setStringList(Controller::instance().simulation()->scripts());
    m_network_scripts.setStringList(Controller::instance().simulation()->network()->scripts());
}

void ScriptsWindow::simulation_changed(Simulation*){
    read_name_lists();
}

void ScriptsWindow::on_simulationPlusButton_clicked(){
    QString name = "new script";
    QString postfix = "";
    QStringList names = Controller::instance().simulation()->scripts();
    int i=0;
    while(names.contains(name+postfix)){
        i++;
        postfix = QString(" %1").arg(i);
    }
    Controller::instance().simulation()->set_script(name+postfix, "");
    read_name_lists();
}

void ScriptsWindow::on_simulationMinusButton_clicked(){
    Controller::instance().simulation()->remove_script(m_current_script);
    read_name_lists();
    ui->tabWidget_right->setEnabled(false);
    ui->textEdit->setText("");
}

void ScriptsWindow::on_networkPlusButton_clicked(){
    QString name = "new script";
    QString postfix = "";
    QStringList names = Controller::instance().simulation()->network()->scripts();
    int i=0;
    while(names.contains(name+postfix)){
        i++;
        postfix = QString(" %1").arg(i);
    }
    Controller::instance().simulation()->network()->set_script(name+postfix, "");
    read_name_lists();
}

void ScriptsWindow::on_networkMinusButton_clicked(){
    Controller::instance().simulation()->remove_script(m_current_script);
    read_name_lists();
}

void ScriptsWindow::on_loadButton_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open script"), "", tr("Javascript files (*.js)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    ui->textEdit->setText(file.readAll());
}

void ScriptsWindow::on_saveButton_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save script"), "", tr("Javascript files (*.js)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(ui->textEdit->toPlainText().toUtf8());
}

void ScriptsWindow::on_playButton_clicked(){
    QString return_value;
    QString script_name;
    QModelIndex index;
    switch(ui->tabWidget->currentIndex()){
    case 0:
        if(!ui->simulationScriptsList->selectionModel()) return;
        if(ui->simulationScriptsList->selectionModel()->selectedRows().empty()) return;
        index = ui->simulationScriptsList->selectionModel()->selectedRows().first();
        script_name = m_simulation_scripts.data(index, Qt::DisplayRole).toString();
        return_value = Controller::instance().simulation()->run_script(script_name);
        break;
    case 1:
        if(!ui->networkScriptsList->selectionModel()) return;
        if(ui->networkScriptsList->selectionModel()->selectedRows().empty()) return;
        index = ui->networkScriptsList->selectionModel()->selectedRows().first();
        script_name = m_network_scripts.data(index, Qt::DisplayRole).toString();
        Controller::instance().simulation()->network()->run_script(script_name);
        break;
    }
    ui->outputListWidget->addItem(return_value);
}

void ScriptsWindow::on_pauseButton_clicked(){

}

void ScriptsWindow::textChanged(){
    QString script_name;
    QModelIndex index;
    switch(ui->tabWidget->currentIndex()){
    case 0:
        if(!ui->simulationScriptsList->selectionModel()) return;
        if(ui->simulationScriptsList->selectionModel()->selectedRows().empty()) return;
        index = ui->simulationScriptsList->selectionModel()->selectedRows().first();
        script_name = m_simulation_scripts.data(index, Qt::DisplayRole).toString();
        Controller::instance().simulation()->set_script(script_name, ui->textEdit->toPlainText());
        break;
    case 1:
        if(!ui->networkScriptsList->selectionModel()) return;
        if(ui->networkScriptsList->selectionModel()->selectedRows().empty()) return;
        index = ui->networkScriptsList->selectionModel()->selectedRows().first();
        script_name = m_network_scripts.data(index, Qt::DisplayRole).toString();
        Controller::instance().simulation()->network()->set_script(script_name, ui->textEdit->toPlainText());
    }
}


void ScriptsWindow::simulationScriptsNameChanged(QModelIndex index,QModelIndex){
    QString s = m_simulation_scripts.data(index, Qt::DisplayRole).toString();
    Controller::instance().simulation()->set_script(s, Controller::instance().simulation()->script(m_current_script));
    Controller::instance().simulation()->remove_script(m_current_script);
    m_current_script = s;
    textChanged();
    read_name_lists();
}

void ScriptsWindow::networkScriptsNameChanged(QModelIndex index,QModelIndex){
    QString s = m_network_scripts.data(index, Qt::DisplayRole).toString();
    Controller::instance().simulation()->network()->set_script(s, Controller::instance().simulation()->network()->script(m_current_script));
    Controller::instance().simulation()->network()->remove_script(m_current_script);
    m_current_script = s;
    textChanged();
    read_name_lists();
}

void ScriptsWindow::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    read_name_lists();
}

void ScriptsWindow::script_output(QString output){
    ui->outputListWidget->addItem(output);
}

void ScriptsWindow::on_clearButton_clicked(){
    ui->outputListWidget->clear();
}
