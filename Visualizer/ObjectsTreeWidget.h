#pragma once
#include <QtGui/QTreeWidget>
class VisualizerContext;
class VisualizerData;
class VisualizerData2D;
class VisualizerData2DList;

class ObjectsTreeWidget : public QTreeWidget{
Q_OBJECT
public:
	ObjectsTreeWidget(VisualizerContext* context, QWidget* parent);
	void updateFromContext();

signals:
	void changed(VisualizerData*);

private slots:
	void changedFromEditor(VisualizerData*);
private:
	VisualizerContext* m_context;
	
	void addVisualizerData(VisualizerData* data);
	void addDescription(QTreeWidgetItem* root, VisualizerData* data);
	void addUnit(QTreeWidgetItem* root, VisualizerData* data);
	void addColor(QTreeWidgetItem* root, VisualizerData* data);
	void addVisible(QTreeWidgetItem* root, VisualizerData* data);
	void addTransparency(QTreeWidgetItem* root, VisualizerData* data);
	void addBold(QTreeWidgetItem* root, VisualizerData* data);
	void addOscilloscopeEffect(QTreeWidgetItem* root, VisualizerData2DList* data);
	void addStyle(QTreeWidgetItem* root, VisualizerData* data);
	void addShowMode(QTreeWidgetItem* root, VisualizerData2D* data);
};
