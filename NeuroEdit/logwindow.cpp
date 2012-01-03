#include "logwindow.h"
#include "ui_logwindow.h"
#include <assert.h>
#include <boost/foreach.hpp>

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    connect(&Log::instance(), SIGNAL(new_log_message(QString,Log::LogLevel)), this, SLOT(new_log_message(QString,Log::LogLevel)));
}

LogWindow::~LogWindow()
{
    delete ui;
}


void LogWindow::new_log_message(QString text, Log::LogLevel level){
    m_messages.push_back(std::pair< std::string,Log::LogLevel >(text.toStdString(),level));
    add_message_to_widget(text.toStdString(), level);
}

void LogWindow::on_clearButton_clicked(bool){
    ui->listWidget->clear();
    m_messages.clear();
}

void LogWindow::on_logLevelComboBox_currentIndexChanged(int index){
    Log::LogLevel level;
    switch(index){
    case 0: level = Log::DEBUG; break;
    case 1: level = Log::INFO; break;
    case 2: level = Log::ERROR; break;
    default: assert(false);
    }
    rebuild(level);
}


void LogWindow::rebuild(Log::LogLevel level){
    ui->listWidget->clear();
    std::pair< std::string,Log::LogLevel > it;
    BOOST_FOREACH(it, m_messages){
        if(it.second >= level) add_message_to_widget(it.first, it.second);
    }
}


void LogWindow::add_message_to_widget(std::string message, Log::LogLevel level){
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setText(message.c_str());
    if(level == Log::ERROR) item->setForeground(QBrush(QColor::fromRgb(255,0,0)));
    if(level == Log::DEBUG) item->setForeground(QBrush(QColor::fromRgb(0,0,255)));
    ui->listWidget->addItem(item);
    ui->listWidget->scrollToItem(item);
}
