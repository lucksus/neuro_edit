#include "scriptswindow.h"
#include "ui_scriptswindow.h"
#include "network.h"
#include "simulation.h"
#include "application.h"
#include "controller.h"
#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include <Qsci/qscilexerjavascript.h>

ScriptRunnerThread::ScriptRunnerThread(ScriptsWindow* window)
:m_scripts_window(window){}

ScriptsWindow::ScriptsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptsWindow),
    m_thread(this),
    m_current_history_position(-1)
{
    ui->setupUi(this);

    delete ui->lineEdit;
    ShellLineEdit* shell_line_edit = new ShellLineEdit(this);
    ui->lineEdit = shell_line_edit;
    dynamic_cast<QVBoxLayout*>(ui->verticalLayout_3)->insertWidget(1,ui->lineEdit);
    connect(shell_line_edit, SIGNAL(history_up()), this, SLOT(history_up()));
    connect(shell_line_edit, SIGNAL(history_down()), this, SLOT(history_down()));
    connect(shell_line_edit, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_returnPressed()));

    ui->textEdit->setLexer(new QsciLexerJavaScript(ui->textEdit));

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(&m_timer,SIGNAL(timeout()), this, SLOT(check_for_script_output()));
    m_timer.start(100);
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

void ScriptsWindow::on_loadButton_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open script"), "", tr("Javascript files (*.js)"));
    if(fileName.isEmpty()) return;
    load_script(fileName);
}

void ScriptsWindow::on_saveButton_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save script"), "", tr("Javascript files (*.js)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write(ui->textEdit->text().toUtf8());
}

void ScriptRunnerThread::run(){
    /*QString script_name;
    QModelIndex index;
    index = m_scripts_window->ui->simulationScriptsList->selectionModel()->selectedRows().first();
    script_name = m_scripts_window->m_simulation_scripts.data(index, Qt::DisplayRole).toString();
    QString script = Controller::instance().simulation()->script(script_name);*/
    QString return_value = Controller::instance().script_engine()->evaluate(m_code);
    Controller::instance().output_from_script(return_value);
}

void ScriptsWindow::on_playButton_clicked(){
    m_thread.set_code(ui->textEdit->text());
    m_thread.start();
}

void ScriptsWindow::on_pauseButton_clicked(){
    //Controller::instance().script_engine()->trigger_debugger();
}

void ScriptsWindow::textChanged(){

}


void ScriptsWindow::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    read_name_lists();
}

void ScriptsWindow::check_for_script_output(){
    Controller &c=Controller::instance();
    if(!c.m_mutex_for_buffer.tryLock()) return;
    //QMutexLocker l(&c.m_mutex_for_buffer);
    while(!c.m_script_output_buffer.empty()){
        script_output(c.m_script_output_buffer.front());
        c.m_script_output_buffer.pop_front();
    }
    c.m_mutex_for_buffer.unlock();
}

void ScriptsWindow::script_output(QString output, QColor color){
    ui->output->insertHtml(QString("<font color=%2>%1</font><br>").arg(output).arg(color.name()));
    ui->output->ensureCursorVisible();
}

void ScriptsWindow::on_clearButton_clicked(){
    ui->output->setText("");
}

void ScriptsWindow::on_lineEdit_returnPressed(){
    QString line = ui->lineEdit->text();
    ui->lineEdit->setText("");
    script_output(QString("# %1").arg(line), QColor(0,0,255));
    m_thread.set_code(line);
    m_thread.start();
    //QString return_value = Controller::instance().script_engine()->evaluate(line);
    //script_output(QString("--> %1").arg(return_value));
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

void ScriptsWindow::load_script(QString filename){
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    ui->textEdit->setText(file.readAll());
}
