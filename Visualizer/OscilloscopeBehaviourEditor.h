#pragma once
#include <QCheckBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
class VisualizerData2DList;
class VisualizerData;

class OscilloscopeBehaviourEditor : public QCheckBox{
Q_OBJECT
public:
	OscilloscopeBehaviourEditor(VisualizerData2DList* data,  QTreeWidget *widget, QTreeWidgetItem* parentItem);

signals:
	void changed(VisualizerData*);

private slots:
	void on_stateChanged(int index);

private:
	VisualizerData2DList* m_visualizerDataObject;
};
