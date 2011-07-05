#include "ObjectsTreeWidget.h"
#include <boost/foreach.hpp>
#include <assert.h>
#include "VisualizerCanvas2D.h"
#include "VisualizerContext2D.h"
#include "VisualizerData2DList.h"
#include "VisualizerContext3D.h"
#include "ColorListEditor.h"
#include "VisibleEditor.h"
#include "TransparencyEditor.h"
#include "OscilloscopeBehaviourEditor.h"
#include "StyleEditor.h"
#include "ShowModeEditor.h"
#include "BoldEditor.h"


ObjectsTreeWidget::ObjectsTreeWidget(VisualizerContext* context, QWidget* parent)
: QTreeWidget(parent), m_context(context)
{
	setColumnCount(2);
	QStringList headerLabels;
	headerLabels << "Property" << "Value";
	setHeaderLabels(headerLabels);
	setColumnWidth(0,140);
	updateFromContext();
}


void ObjectsTreeWidget::updateFromContext(){
	clear();
	BOOST_FOREACH(VisualizerData* data, m_context->getData())
		addVisualizerData(data);
}

void ObjectsTreeWidget::addVisualizerData(VisualizerData* data){
	QStringList name;
	name << data->getName().c_str();
	QTreeWidgetItem* root = new QTreeWidgetItem(this, name);
	addTopLevelItem(root);
	addDescription(root,data);
	addUnit(root,data);
	addColor(root,data);
	addVisible(root,data);
	if (dynamic_cast<VisualizerData3D*>(data)) {
		addTransparency(root, data);
		addBold(root, data);
	}
	if(VisualizerData2DList* data2DList = dynamic_cast<VisualizerData2DList*>(data))
		addOscilloscopeEffect(root, data2DList);
	else{
		addStyle(root,data);
		if(VisualizerData2D* data2D = dynamic_cast<VisualizerData2D*>(data))
			addShowMode(root,data2D);
	}
}

void ObjectsTreeWidget::addDescription(QTreeWidgetItem* root, VisualizerData* data){
	QStringList strings;
	strings << "Description";
	strings << data->getDescription().c_str();
	new QTreeWidgetItem(root,strings);
}

void ObjectsTreeWidget::addUnit(QTreeWidgetItem* root, VisualizerData* data){
	QStringList strings;
	strings << "y-Axis Unit";
	strings << data->getUnit().c_str();
	new QTreeWidgetItem(root,strings);
}

void ObjectsTreeWidget::addColor(QTreeWidgetItem* root, VisualizerData* data){
	QStringList strings;
	strings << "Color";
	QTreeWidgetItem* item = new QTreeWidgetItem(root,strings);
	ColorListEditor* editor = new ColorListEditor(data,this,item);
	connect(editor,SIGNAL(changed(VisualizerData*)),this,SLOT(changedFromEditor(VisualizerData*)));
}

void ObjectsTreeWidget::addVisible(QTreeWidgetItem* root, VisualizerData* data){
	QStringList strings;
	strings << "Visible";
	QTreeWidgetItem* item = new QTreeWidgetItem(root,strings);
	VisibleEditor* editor = new VisibleEditor(data,this,item);
	connect(editor,SIGNAL(changed(VisualizerData*)),this,SLOT(changedFromEditor(VisualizerData*)));
}

void ObjectsTreeWidget::addBold(QTreeWidgetItem* root, VisualizerData* data){
	QStringList strings;
	strings << "Bold";
	QTreeWidgetItem* item = new QTreeWidgetItem(root,strings);
	BoldEditor* editor = new BoldEditor(data,this,item);
	connect(editor,SIGNAL(changed(VisualizerData*)),this,SLOT(changedFromEditor(VisualizerData*)));
}

void ObjectsTreeWidget::addTransparency(QTreeWidgetItem* root, VisualizerData* data) {
	QStringList strings;
	strings << "Intensity-induced transparency";
	QTreeWidgetItem* item = new QTreeWidgetItem(root, strings);
	TransparencyEditor* editor = new TransparencyEditor(data, this, item);
	connect(editor, SIGNAL(changed(VisualizerData*)), this, SLOT(changedFromEditor(VisualizerData*)));
}

void ObjectsTreeWidget::addOscilloscopeEffect(QTreeWidgetItem* root, VisualizerData2DList* data){
	QStringList strings;
	strings << "Oscilloscope effect";
	QTreeWidgetItem* item = new QTreeWidgetItem(root,strings);
	OscilloscopeBehaviourEditor* editor = new OscilloscopeBehaviourEditor(data,this,item);
	connect(editor,SIGNAL(changed(VisualizerData*)),this,SLOT(changedFromEditor(VisualizerData*)));
}

void ObjectsTreeWidget::addStyle(QTreeWidgetItem* root, VisualizerData* data){
	QStringList strings;
	strings << "Line style";
	QTreeWidgetItem* item = new QTreeWidgetItem(root,strings);
	StyleEditor* editor = new StyleEditor(data,this,item);
	connect(editor,SIGNAL(changed(VisualizerData*)),this,SLOT(changedFromEditor(VisualizerData*)));
}

void ObjectsTreeWidget::addShowMode(QTreeWidgetItem* root, VisualizerData2D* data){
	QStringList strings;
	strings << "Show";
	QTreeWidgetItem* item = new QTreeWidgetItem(root,strings);
	ShowModeEditor* editor = new ShowModeEditor(data,this,item);
	connect(editor,SIGNAL(changed(VisualizerData*)),this,SLOT(changedFromEditor(VisualizerData*)));
}

void ObjectsTreeWidget::changedFromEditor(VisualizerData* data){
	emit changed(data);
}