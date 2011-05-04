#include "scriptswindow.h"
#include "ui_scriptswindow.h"
#include "network.h"

ScriptsWindow::ScriptsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptsWindow),
    m_network(0)
{
    ui->setupUi(this);
    ui->listView->setModel(&m_string_list_model);
    m_syntax_highlighter = new ScriptSyntaxHighlighter(ui->textEdit);
}

ScriptsWindow::~ScriptsWindow()
{
    delete ui;
}

void ScriptsWindow::set_network(Network *n){
    m_network = n;
    m_string_list_model.setStringList(n->scripts());
}

void ScriptsWindow::on_plusButton_clicked(){

}

void ScriptsWindow::on_minusButton_clicked(){

}

void ScriptsWindow::on_saveButton_clicked(){

}
