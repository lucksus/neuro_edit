#ifndef LOGWINDOW_H
#define LOGWINDOW_H
#include "log.h"

#include <QWidget>
#include <QtCore/QTimer>

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
    void check_for_new_messages();

    void on_clearButton_clicked(bool);
    void on_logLevelComboBox_currentIndexChanged(int index);

private:
    Ui::LogWindow *ui;
    long m_last_message_index;
    QTimer m_timer;

    void rebuild(Log::LogLevel level);
    void add_message_to_widget(std::string message, Log::LogLevel level);
    Log::LogLevel combobox_index_to_log_level(int index);
};

#endif // LOGWINDOW_H
