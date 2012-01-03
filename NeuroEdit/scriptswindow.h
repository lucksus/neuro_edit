#ifndef SCRIPTSWINDOW_H
#define SCRIPTSWINDOW_H

#include <QWidget>
#include <QtGui/QStringListModel>
#include "scriptsyntaxhighlighter.h"
#include <QtGui/QListView>

namespace Ui {
    class ScriptsWindow;
}

class MyListView : public QListView{
Q_OBJECT
public:
    MyListView(QWidget* parent):QListView(parent){}
signals:
    void selection_changed(const QItemSelection &selected, const QItemSelection &deselected);
protected:
    virtual void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
        emit selection_changed(selected, deselected);
    }
};

class Simulation;
class ScriptsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ScriptsWindow(QWidget *parent = 0);
    ~ScriptsWindow();

protected slots:
    void on_simulationPlusButton_clicked();
    void on_simulationMinusButton_clicked();
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_clearButton_clicked();

    void simulation_changed(Simulation*);

    void simulationScriptSelected(const QItemSelection & selected, const QItemSelection & deselected);

    void textChanged();
    void simulationScriptsNameChanged(QModelIndex,QModelIndex);
    void script_output(QString);

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::ScriptsWindow *ui;
    QStringListModel m_simulation_scripts;
    QStringListModel m_network_scripts;
    ScriptSyntaxHighlighter* m_syntax_highlighter;
    QString m_current_script;

    void read_name_lists();
};

#endif // SCRIPTSWINDOW_H
