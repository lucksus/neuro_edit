#ifndef SCRIPTSWINDOW_H
#define SCRIPTSWINDOW_H

#include <QWidget>
#include <QtGui/QStringListModel>
#include "scriptsyntaxhighlighter.h"

namespace Ui {
    class ScriptsWindow;
}

class Network;
class ScriptsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ScriptsWindow(QWidget *parent = 0);
    ~ScriptsWindow();

    void set_network(Network*);

protected slots:
    void on_plusButton_clicked();
    void on_minusButton_clicked();
    void on_saveButton_clicked();

private:
    Ui::ScriptsWindow *ui;
    Network* m_network;
    QStringListModel m_string_list_model;
    ScriptSyntaxHighlighter* m_syntax_highlighter;
};

#endif // SCRIPTSWINDOW_H
