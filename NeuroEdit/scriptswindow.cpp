#include "scriptswindow.h"
#include "ui_scriptswindow.h"
#include "network.h"
#include "simulation.h"
#include "application.h"
#include "controller.h"
#include <QtCore/QFile>
#include <QtGui/QFileDialog>

ScriptRunnerThread::ScriptRunnerThread(ScriptsWindow* window)
:m_scripts_window(window){}

ScriptsWindow::ScriptsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptsWindow),
    m_thread(this),
    m_current_history_position(-1)
{
    ui->setupUi(this);
    delete ui->simulationScriptsList;
    ui->simulationScriptsList = new MyListView(this);
    dynamic_cast<QBoxLayout*>(ui->groupBox->layout())->insertWidget(1,ui->simulationScriptsList);

    delete ui->lineEdit;
    ShellLineEdit* shell_line_edit = new ShellLineEdit(this);
    ui->lineEdit = shell_line_edit;
    dynamic_cast<QVBoxLayout*>(ui->verticalLayout_3)->insertWidget(1,ui->lineEdit);
    connect(shell_line_edit, SIGNAL(history_up()), this, SLOT(history_up()));
    connect(shell_line_edit, SIGNAL(history_down()), this, SLOT(history_down()));
    connect(shell_line_edit, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_returnPressed()));

    ui->simulationScriptsList->setModel(&m_simulation_scripts);
    ui->simulationScriptsList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_syntax_highlighter = new ScriptSyntaxHighlighter(ui->textEdit);

    connect(ui->simulationScriptsList, SIGNAL(selection_changed(const QItemSelection&, const QItemSelection &)), this, SLOT(simulationScriptSelected(QItemSelection,QItemSelection)));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(&m_simulation_scripts, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(simulationScriptsNameChanged(QModelIndex,QModelIndex)));
    //connect(ui->simulationScriptsList, SIGNAL(activated(QModelIndex)), this, SLOT(simulationScriptSelected(QModelIndex)));
    connect(&Controller::instance(), SIGNAL(script_output(QString)), this, SLOT(script_output(QString)),Qt::QueuedConnection);
}

void ScriptsWindow::simulationScriptSelected(const QItemSelection & selected, const QItemSelection &){
    QModelIndex index = selected.indexes().first();
    if(QModelIndex() == index){
        ui->splitter->setEnabled(false);
        ui->textEdit->setText("");
        return;
    }
    ui->splitter->setEnabled(true);
    ui->textEdit->setEnabled(true);
    QString script_name = m_simulation_scripts.data(index, Qt::DisplayRole).toString();
    ui->textEdit->setText(Controller::instance().simulation()->script(script_name));
    m_current_script = script_name;
}

ScriptsWindow::~ScriptsWindow()
{
    delete ui;
}

void ScriptsWindow::read_name_lists(){
    if(Controller::instance().simulation() == 0) return;
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
    ui->splitter->setEnabled(false);
    ui->textEdit->setText("");
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

void ScriptRunnerThread::run(){
    QString script_name;
    QModelIndex index;
    index = m_scripts_window->ui->simulationScriptsList->selectionModel()->selectedRows().first();
    script_name = m_scripts_window->m_simulation_scripts.data(index, Qt::DisplayRole).toString();
    QString return_value = Controller::instance().simulation()->run_script(script_name);
    m_scripts_window->ui->outputListWidget->addItem(return_value);
}

void ScriptsWindow::on_playButton_clicked(){
    if(!ui->simulationScriptsList->selectionModel()) return;
    if(ui->simulationScriptsList->selectionModel()->selectedRows().empty()) return;
    m_thread.start();
}

void ScriptsWindow::on_pauseButton_clicked(){

}

void ScriptsWindow::textChanged(){
    QString script_name;
    QModelIndex index;
    if(!ui->simulationScriptsList->selectionModel()) return;
    if(ui->simulationScriptsList->selectionModel()->selectedRows().empty()) return;
    index = ui->simulationScriptsList->selectionModel()->selectedRows().first();
    script_name = m_simulation_scripts.data(index, Qt::DisplayRole).toString();
    Controller::instance().simulation()->set_script(script_name, ui->textEdit->toPlainText());
}


void ScriptsWindow::simulationScriptsNameChanged(QModelIndex index,QModelIndex){
    QString s = m_simulation_scripts.data(index, Qt::DisplayRole).toString();
    Controller::instance().simulation()->set_script(s, Controller::instance().simulation()->script(m_current_script));
    Controller::instance().simulation()->remove_script(m_current_script);
    m_current_script = s;
    textChanged();
    read_name_lists();
}

void ScriptsWindow::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    read_name_lists();
}

void ScriptsWindow::script_output(QString output, QColor color){
    QListWidgetItem* item = new QListWidgetItem(ui->outputListWidget);
    item->setText(output);
    item->setForeground(color);
    ui->outputListWidget->addItem(item);
    ui->outputListWidget->scrollToItem(item);
}

void ScriptsWindow::on_clearButton_clicked(){
    ui->outputListWidget->clear();
}

void ScriptsWindow::on_lineEdit_returnPressed(){
    QString line = ui->lineEdit->text();
    ui->lineEdit->setText("");
    script_output(QString("# %1").arg(line), QColor(0,0,255));
    QString return_value = Controller::instance().simulation()->evaluate_code(line);
    script_output(QString("--> %1").arg(return_value));
    m_shell_history.prepend(line);
    m_current_history_position = -1;
}

void ShellLineEdit::keyPressEvent ( QKeyEvent * event ){
    QLineEdit::keyPressEvent(event);
    if(event->key() == Qt::Key_Up){
        emit history_up();
    }

    if(event->key() == Qt::Key_Down){
        emit history_down();
    }
}

void ScriptsWindow::history_up(){
    m_current_history_position++;
    if(m_current_history_position >= m_shell_history.size()){
        m_current_history_position--;
        return;
    }
    ui->lineEdit->setText(m_shell_history.at(m_current_history_position));
    ui->lineEdit->setCursorPosition(ui->lineEdit->text().size());
}

void ScriptsWindow::history_down(){
    m_current_history_position--;
    if(m_current_history_position < 0){
        ui->lineEdit->setText("");
        m_current_history_position = -1;
        return;
    }
    ui->lineEdit->setText(m_shell_history.at(m_current_history_position));
    ui->lineEdit->setCursorPosition(ui->lineEdit->text().size());
}
