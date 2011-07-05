#include "StyleEditor.h"
#include "VisualizerData2D.h"

StyleEditor::StyleEditor(VisualizerData* data, QTreeWidget *widget, QTreeWidgetItem* parentItem)
: QComboBox(widget), m_visualizerDataObject(data)
{
	widget->setItemWidget(parentItem,1,this);
	insertItem(0, "Line");
	setItemData(0, VisualizerData::LINE, Qt::DecorationRole);
	insertItem(1, "Cross");
	setItemData(1, VisualizerData::CROSS, Qt::DecorationRole);
	insertItem(2, "Dashed line");
	setItemData(2, VisualizerData::DASHED_LINE, Qt::DecorationRole);
	insertItem(3, "Dashed dotted line");
	setItemData(3, VisualizerData::DASHED_DOTTED_LINE, Qt::DecorationRole);

	if ( dynamic_cast<VisualizerData2D*>(data) ) {
		insertItem(4, "Square");
		setItemData(4, VisualizerData::SQUARE, Qt::DecorationRole);
	}

	setValue(data->getStyle());
	connect(this,SIGNAL(activated(int)),this,SLOT(on_currentIndexChanged(int)));
}

VisualizerData::Style StyleEditor::value() const
{
	return VisualizerData::Style(itemData(currentIndex(), Qt::DecorationRole).value<int>());
}

void StyleEditor::setValue(VisualizerData::Style b)
{
	setCurrentIndex(findData(b, int(Qt::DecorationRole)));
}

void StyleEditor::on_currentIndexChanged(int){
	m_visualizerDataObject->setStyle(value());
	emit changed(m_visualizerDataObject);
}
