#pragma once
#include <QComboBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "VisualizerData.h"

class StyleEditor : public QComboBox{
Q_OBJECT
public:
	StyleEditor(VisualizerData* data,QTreeWidget *widget, QTreeWidgetItem* parentItem);

	VisualizerData::Style value() const;
	void setValue(VisualizerData::Style b);

signals:
	void changed(VisualizerData*);

private slots:
	void on_currentIndexChanged(int index);

private:
	VisualizerData* m_visualizerDataObject;

};
