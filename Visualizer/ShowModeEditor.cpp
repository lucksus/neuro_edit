#include "ShowModeEditor.h"

ShowModeEditor::ShowModeEditor(VisualizerData2D* data, QTreeWidget *widget, QTreeWidgetItem* parentItem)
: QComboBox(widget), m_visualizerDataObject(data)
{
	widget->setItemWidget(parentItem,1,this);
	insertItem(0, "Real");
	setItemData(0, VisualizerData2D::REAL, Qt::DecorationRole);
	insertItem(1, "Imaginary");
	setItemData(1, VisualizerData2D::IMAG, Qt::DecorationRole);
	insertItem(2, "Absolute value");
	setItemData(2, VisualizerData2D::ABS, Qt::DecorationRole);
	
	setCurrentIndex(findData(data->getStyle(), int(Qt::DecorationRole)));
	connect(this,SIGNAL(activated(int)),this,SLOT(on_currentIndexChanged(int)));
}

void ShowModeEditor::on_currentIndexChanged(int){
	m_visualizerDataObject->setShowMode(VisualizerData2D::MODE(qVariantValue<int>(itemData(currentIndex(), Qt::DecorationRole))));
	emit changed(m_visualizerDataObject);
}
