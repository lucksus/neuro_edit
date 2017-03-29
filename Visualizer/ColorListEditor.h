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

#ifndef COLORLISTEDITOR_H
#define COLORLISTEDITOR_H

#include <QComboBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
class QColor;
class QWidget;
class VisualizerData;

class ColorListEditor : public QComboBox
{
	Q_OBJECT
		Q_PROPERTY(QColor color READ color WRITE setColor USER true)

public:
	ColorListEditor(VisualizerData* visualizerData, QTreeWidget *widget, QTreeWidgetItem* parentItem);

	QColor color() const;
	void setColor(QColor c);

signals:
	void changed(VisualizerData*);

private slots:
	void on_currentIndexChanged(int index);

private:
	void populateList();
	VisualizerData* m_visualizerDataObject;
	

};

#endif 
