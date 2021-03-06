#ifndef SCRIPTSWINDOW_H
#define SCRIPTSWINDOW_H

#include <QWidget>
#include <QStringListModel>
#include "scriptsyntaxhighlighter.h"
#include <QListView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QThread>
#include <QTimer>

namespace Ui {
    class ScriptsWindow;
}

class ShellLineEdit : public QLineEdit{
Q_OBJECT
public:
    ShellLineEdit(QWidget* parent):QLineEdit(parent){}
signals:
    void history_up();
    void history_down();
protected:
    virtual void	keyPressEvent ( QKeyEvent * event );
};

class ScriptsWindow;

class ScriptRunnerThread : public QThread{
public:
    ScriptRunnerThread(ScriptsWindow*);
    void set_code(QString code){m_code=code;}
    void run();
private:
    ScriptsWindow *m_scripts_window;
    QString m_code;
};

class Simulation;
class ScriptsWindow : public QWidget
{
    Q_OBJECT
friend class ScriptRunnerThread;
public:
    explicit ScriptsWindow(QWidget *parent = 0);
    ~ScriptsWindow();

    void load_script(QString filename);

protected slots:
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_clearButton_clicked();
    void on_lineEdit_returnPressed();
    void simulation_changed(Simulation*);
    void textChanged();
    void script_output(QString, QColor color = QColor(0,0,0));
    void check_for_script_output();
    void history_up();
    void history_down();

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::ScriptsWindow *ui;
    ScriptRunnerThread m_thread;
    QStringListModel m_simulation_scripts;
    QStringListModel m_network_scripts;
    ScriptSyntaxHighlighter* m_syntax_highlighter;
    QString m_current_script;
    QStringList m_shell_history;
    int m_current_history_position;
    QTimer m_timer;

    void read_name_lists();
};

#endif // SCRIPTSWINDOW_H
