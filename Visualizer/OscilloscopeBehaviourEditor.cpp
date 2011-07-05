#include "OscilloscopeBehaviourEditor.h"
#include "VisualizerData2DList.h"

OscilloscopeBehaviourEditor::OscilloscopeBehaviourEditor(VisualizerData2DList* data,  QTreeWidget *widget, QTreeWidgetItem* parentItem)
: QCheckBox(widget), m_visualizerDataObject(data)
{
	widget->setItemWidget(parentItem,1,this);
	if(data->behaveLikeLabOscilloscope())
		setCheckState(Qt::Checked);
	else
		setCheckState(Qt::Unchecked);

	connect(this,SIGNAL(stateChanged(int)),this,SLOT(on_stateChanged(int)));
}


void OscilloscopeBehaviourEditor::on_stateChanged(int state){
	m_visualizerDataObject->setBehaveLikeLabOscilloscope(state == Qt::Checked);
	emit changed(m_visualizerDataObject);
}