/****************************************************************************
**
** Copyright (C) 2007-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>

#include "ColorListEditor.h"
#include "VisualizerData.h"

ColorListEditor::ColorListEditor(VisualizerData* visualizerData, QTreeWidget *widget, QTreeWidgetItem* parentItem)
: QComboBox(widget), m_visualizerDataObject(visualizerData)
{
	populateList();
	widget->setItemWidget(parentItem,1,this);
	setColor(QColor(m_visualizerDataObject->getColorRed()*255, m_visualizerDataObject->getColorGreen()*255, m_visualizerDataObject->getColorBlue()*255));
	connect(this,SIGNAL(activated(int)),this,SLOT(on_currentIndexChanged(int)));
}

QColor ColorListEditor::color() const
{
	return qVariantValue<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}

void ColorListEditor::setColor(QColor color)
{
	setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

void ColorListEditor::populateList()
{
	QStringList colorNames = QColor::colorNames();

	for (int i = 0; i < colorNames.size(); ++i) {
		QColor color(colorNames[i]);

		insertItem(i, colorNames[i]);
		setItemData(i, color, Qt::DecorationRole);
	}
}

void ColorListEditor::on_currentIndexChanged(int index){
	QStringList colorNames = QColor::colorNames();
	QColor color(colorNames[index]);
	int r,g,b;
	color.getRgb(&r,&g,&b);
	m_visualizerDataObject->setColorInt(r,g,b);
	emit changed(m_visualizerDataObject);
}
