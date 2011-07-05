#include "TransparencyEditor.h"
#include "VisualizerData.h"

TransparencyEditor::TransparencyEditor(VisualizerData* data, QTreeWidget *widget, QTreeWidgetItem* parentItem)
: QCheckBox(widget), m_visualizerDataObject(data)
{
	widget->setItemWidget(parentItem,1,this);
	if(data->isTransparent())
		setCheckState(Qt::Checked);
	else
		setCheckState(Qt::Unchecked);

	connect(this,SIGNAL(stateChanged(int)),this,SLOT(on_stateChanged(int)));
}


void TransparencyEditor::on_stateChanged(int state){
	m_visualizerDataObject->setTransparent(state == Qt::Checked);
	emit changed(m_visualizerDataObject);
}