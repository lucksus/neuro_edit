#include "VisibleEditor.h"
#include "VisualizerData.h"

VisibleEditor::VisibleEditor(VisualizerData* data, QTreeWidget *widget, QTreeWidgetItem* parentItem)
: QCheckBox(widget), m_visualizerDataObject(data)
{
	widget->setItemWidget(parentItem,1,this);
	if(data->isVisible())
		setCheckState(Qt::Checked);
	else
		setCheckState(Qt::Unchecked);

	connect(this,SIGNAL(stateChanged(int)),this,SLOT(on_stateChanged(int)));
}


void VisibleEditor::on_stateChanged(int state){
	m_visualizerDataObject->setVisible(state == Qt::Checked);
	emit changed(m_visualizerDataObject);
}