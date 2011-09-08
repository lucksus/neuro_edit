#ifndef LOGWINDOW_H
#define LOGWINDOW_H
#include "log.h"

#include <QWidget>

namespace Ui {
    class LogWindow;
}

class LogWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();

public slots:
    void new_log_message(QString text, Log::LogLevel level);

    void on_actionClearButton_clicked(bool);
    void on_logLevelComboBox_currentIndexChanged(int index);

private:
    Ui::LogWindow *ui;
    std::list< std::pair<std::string,Log::LogLevel> > m_messages;

    void rebuild(Log::LogLevel level);

    void add_message_to_widget(std::string message, Log::LogLevel level);
};

#endif // LOGWINDOW_H
