#include "BoldEditor.h"
#include "VisualizerData3D.h"

BoldEditor::BoldEditor(VisualizerData* data, QTreeWidget *widget, QTreeWidgetItem* parentItem)
: QCheckBox(widget), m_visualizerDataObject(data)
{
	VisualizerData3D* data3D = dynamic_cast<VisualizerData3D*>(data);
	Q_ASSERT( data3D );

	widget->setItemWidget(parentItem,1,this);
	if(data3D->isBold())
		setCheckState(Qt::Checked);
	else
		setCheckState(Qt::Unchecked);

	connect(this,SIGNAL(stateChanged(int)),this,SLOT(on_stateChanged(int)));
}


void BoldEditor::on_stateChanged(int state){
	dynamic_cast<VisualizerData3D*>( m_visualizerDataObject )->setBold(state == Qt::Checked);
	emit changed(m_visualizerDataObject);
}