#include "logwindow.h"
#include "ui_logwindow.h"
#include <assert.h>
#include <boost/foreach.hpp>

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWindow),
    m_last_message_index(0),
    m_timer(this)
{
    ui->setupUi(this);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(check_for_new_messages()));
    m_timer.start(100);
}

LogWindow::~LogWindow()
{
    delete ui;
}


void LogWindow::on_clearButton_clicked(bool){
    ui->listWidget->clear();
}

void LogWindow::on_logLevelComboBox_currentIndexChanged(int index){
    rebuild(combobox_index_to_log_level(index));
}


void LogWindow::rebuild(Log::LogLevel level){
    ui->listWidget->clear();
    std::pair< std::string,Log::LogLevel > it;
    BOOST_FOREACH(it, Log::instance().messages()){
        if(it.second >= level) add_message_to_widget(it.first, it.second);
    }
    m_last_message_index = Log::instance().messages().size()-1;
}


void LogWindow::add_message_to_widget(std::string message, Log::LogLevel level){
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setText(message.c_str());
    if(level == Log::ERROR) item->setForeground(QBrush(QColor::fromRgb(255,0,0)));
    if(level == Log::DEBUG) item->setForeground(QBrush(QColor::fromRgb(0,0,255)));
    ui->listWidget->addItem(item);
    ui->listWidget->scrollToItem(item);
}

void LogWindow::check_for_new_messages(){
    const std::vector<std::pair<std::string, Log::LogLevel> >& messages = Log::instance().messages();
    int message_count = messages.size();
    if(! (message_count-1 > m_last_message_index)) return;
    Log::LogLevel level = combobox_index_to_log_level(ui->logLevelComboBox->currentIndex());
    for(unsigned long i = m_last_message_index+1; i<messages.size(); i++){
        if(messages[i].second >= level) add_message_to_widget(messages[i].first, messages[i].second);
    }
    m_last_message_index = messages.size()-1;
}

Log::LogLevel LogWindow::combobox_index_to_log_level(int index){
    Log::LogLevel level;
    switch(index){
    case 0: level = Log::DEBUG; break;
    case 1: level = Log::INFO; break;
    case 2: level = Log::ERROR; break;
    default: assert(false);
    }
    return level;
}
