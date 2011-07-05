#include "VisualizerWidget.h"
#include <QGridLayout>
#include <assert.h>
#include "VisualizerData.h"
#include "VisualizerCanvas2D.h"
#include "VisualizerContext2D.h"
#include "VisualizerCanvasPoincare.h"
#include "VisualizerContext3D.h"
#include "VisualizerContextMatlabExporter.h"
#include "VisualizerContextAsciiExporter.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QItemDelegate>
#include "ColorListEditor.h"
#include <QMetaProperty>
#include <QItemEditorFactory>
#include <QItemEditorCreatorBase>
#include <QCheckBox>
#include "BoolEditor.h"
#include "StyleEditor.h"
#include "ObjectsTreeWidget.h"
#include <QKeyEvent>
#include <boost/foreach.hpp>

VisualizerWidget::VisualizerWidget(VisualizerContext* context, QWidget *parent)
: QMainWindow(parent), m_context(context), m_canvas(0)
{
	ui.setupUi(this);

	m_treeWidget = new ObjectsTreeWidget(m_context,this);
	ui.objectsBox->layout()->addWidget(m_treeWidget);

	ui.glViewBox->setLayout(new QGridLayout());
	
	initializeCanvasAndGUI(context);
}

void VisualizerWidget::initializeCanvasAndGUI(VisualizerContext* context) {
	VisualizerContext2D* context_2d = dynamic_cast<VisualizerContext2D*>(context);
	VisualizerContext3D* context_3d = dynamic_cast<VisualizerContext3D*>(context);

	if		( context_2d ) m_canvas = new VisualizerCanvas2D(this, context_2d);
	else if ( context_3d ) m_canvas = new VisualizerCanvasPoincare(this, context_3d);
	else assert(false);

	BOOST_FOREACH( VisualizerData* d, context->getData() ) {
		m_canvas->addData(d);
	}

	ui.glViewBox->setLayout(new QVBoxLayout);
	ui.glViewBox->layout()->addWidget(m_canvas);

	if ( context_3d ) {
		ui.actionAuswahlmodus->setEnabled(false);
		ui.actionTo_Matlab->setEnabled(false);
		ui.actionTo_Ascii->setEnabled(false);
		ui.actionZoom->setEnabled(false);
	}	

	connect(m_canvas, SIGNAL(newMouseCoordinates(float,float)), this, SLOT(showMouseCoordinatesInStatusBar(float,float)));
	connect(m_canvas, SIGNAL(newPickerCoordinates(float,float,float,float)), this, SLOT(updatePickerCoordinates(float,float,float,float)));
	connect(m_canvas, SIGNAL(switchedToNavigationMode()), this, SLOT(canvasSwitchedToNavigationMode()));
	connect(ui.actionAlign, SIGNAL(triggered()), m_canvas, SLOT(snapViewAroundData()));
	qRegisterMetaType<VisualizerData*>("VisualizerData*");
	connect(m_treeWidget, SIGNAL(changed(VisualizerData*)), m_canvas, SLOT(updateVisualizerData(VisualizerData*)));
}

VisualizerWidget::~VisualizerWidget()
{
	if (m_canvas) delete m_canvas;
}


void VisualizerWidget::updateFromContext(){
	m_treeWidget->updateFromContext();

	m_canvas->clearData();
	vector<VisualizerData*> data = m_context->getData();
	BOOST_FOREACH( VisualizerData* d, data ) {
		m_canvas->addData(d);
	}

	for ( unsigned int i = 0; i < m_context->getAxes().size(); ++i ) {
		VisualizerContext::Axis a = m_context->getAxes()[i];
		QString s = QString("%1 in %2").arg(a.name.c_str()).arg(a.unit.c_str());
		m_canvas->addAxisDescription( s.toStdString() );
	}
}

void VisualizerWidget::on_actionNavigationsmodus_triggered(bool){
	if ( !dynamic_cast<VisualizerContext2D*>(m_context) ) {
		ui.actionNavigationsmodus->setChecked(true);
		return;
	}
	m_canvas->setNavigationMode();
	ui.actionAuswahlmodus->setChecked(false);
	ui.actionZoom->setChecked(false);
	ui.actionNavigationsmodus->setChecked(true);
}

void VisualizerWidget::on_actionAuswahlmodus_triggered(bool){
	if ( !dynamic_cast<VisualizerContext2D*>(m_context) ) return;
	m_canvas->setPickerMode();
	ui.actionZoom->setChecked(false);
	ui.actionNavigationsmodus->setChecked(false);
	ui.actionAuswahlmodus->setChecked(true);
}

void VisualizerWidget::on_actionZoom_triggered(bool){
	if ( !dynamic_cast<VisualizerContext2D*>(m_context) ) return;
	m_canvas->setZoomMode();
	ui.actionNavigationsmodus->setChecked(false);
	ui.actionAuswahlmodus->setChecked(false);
	ui.actionZoom->setChecked(true);
}

void VisualizerWidget::showMouseCoordinatesInStatusBar(float x, float y){
	statusBar()->showMessage(QString("x: %1 y: %2").arg(x).arg(y));
}

void VisualizerWidget::updatePickerCoordinates(float x1, float y1, float x2, float y2){
	ui.x1Label->setText(QString("%1").arg(x1));
	ui.y1Label->setText(QString("%1").arg(y1));
	ui.x2Label->setText(QString("%1").arg(x2));
	ui.y2Label->setText(QString("%1").arg(y2));
	ui.xDiffLabel->setText(QString("%1").arg(x1-x2));
	ui.yDiffLabel->setText(QString("%1").arg(y1-y2));
}

void VisualizerWidget::show(){
	m_canvas->refresh();
	m_treeWidget->updateFromContext();
	QWidget::show();
}

void VisualizerWidget::on_actionTo_Matlab_triggered(){
	VisualizerCanvas2D* m_canvas_2d = dynamic_cast<VisualizerCanvas2D*>(m_canvas);
	if (!m_canvas_2d) return;

	VisualizerContextMatlabExporter exporter(m_context);
	exporter.doExport(m_canvas_2d->getXMinOfVisualRange(), m_canvas_2d->getXMaxOfVisualRange(), m_canvas_2d->getYMinOfVisualRange(), m_canvas_2d->getYMaxOfVisualRange());
}

void VisualizerWidget::on_actionTo_Ascii_triggered(){
	VisualizerContextAsciiExporter exporter(m_context);
	exporter.doExport();
}

void VisualizerWidget::on_actionTo_Image_triggered(){
	assert(m_canvas);

	QString fileName = QFileDialog::getSaveFileName(this, tr("Export Visualizer to Image"),
		"",
		QString("Windows Bitmap (*.bmp);;")+
		QString("Joint Photographic Experts Group (*.jpg *.jpeg);;")+
		QString("Portable Network Graphics (*.png);;")+
		QString("Portable Pixmap (*.ppm);;")+
		QString("Tagged Image File Format (*.tiff);;")+
		QString("X11 Bitmap (*.xbm);;")+
		QString("X11 Pixmap (*.xpm)")); 
	if(fileName.isEmpty()) return;

	QImage image = m_canvas->grabFrameBuffer(true);

	if(!image.save(fileName)){
		QMessageBox msg(QMessageBox::Critical,"Visualizer","Unable to write image to " + fileName, QMessageBox::Ok);
		msg.setWindowIcon(QIcon(":/Visualizer/photoss.png"));
		msg.exec();
	}
};

void VisualizerWidget::canvasSwitchedToNavigationMode(){
	ui.actionNavigationsmodus->setChecked(true);
	on_actionNavigationsmodus_triggered(true);
}

void VisualizerWidget::keyReleaseEvent(QKeyEvent *event){
	if(event->key()==Qt::Key_Escape){
		event->accept();
		close();
	}
}