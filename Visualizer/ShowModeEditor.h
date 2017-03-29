#pragma once
#include <QComboBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "VisualizerData2D.h"

class ShowModeEditor : public QComboBox{
Q_OBJECT
public:
	ShowModeEditor(VisualizerData2D* data, QTreeWidget *widget, QTreeWidgetItem* parentItem);

signals:
	void changed(VisualizerData*);

private slots:
	void on_currentIndexChanged(int index);

private:
	VisualizerData2D* m_visualizerDataObject;

};
