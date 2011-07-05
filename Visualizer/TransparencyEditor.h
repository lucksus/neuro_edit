#pragma once
#include <QCheckBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
class VisualizerData;

class TransparencyEditor : public QCheckBox{
	Q_OBJECT
public:
	TransparencyEditor(VisualizerData* data,  QTreeWidget *widget, QTreeWidgetItem* parentItem);

signals:
	void changed(VisualizerData*);

	private slots:
		void on_stateChanged(int index);

private:
	VisualizerData* m_visualizerDataObject;
};