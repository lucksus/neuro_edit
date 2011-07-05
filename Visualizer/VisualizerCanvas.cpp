#include "VisualizerCanvas.h"
#include "VisualizerContext.h"

VisualizerCanvas::VisualizerCanvas(QWidget* parent, VisualizerContext* context)
: QGLWidget(parent)
{
	// Hintergrundfarbe aus dem Kontext übernehmen:
	setBackgroundColor( context->getBackgroundRed(), context->getBackgroundGreen(), context->getBackgroundBlue() );

	// Die anzuzeigenden Daten ebenfalls:
	vector<VisualizerData*> data = context->getData();
	vector<VisualizerData*>::iterator data_iterator = data.begin();
	for (; data_iterator != data.end(); ++data_iterator) {
		addData( *data_iterator );
	}
}

VisualizerCanvas::~VisualizerCanvas() {}

void VisualizerCanvas::setBackgroundColor(float red, float green, float blue){
	m_backgroundColor.red	= red;
	m_backgroundColor.green = green;
	m_backgroundColor.blue	= blue;
}

VisualizerCanvas::RGBColor VisualizerCanvas::getBackgroundColor() {
	return m_backgroundColor;
}

void VisualizerCanvas::addData(VisualizerData* data) {
	m_data.push_back(data);
}

const vector<VisualizerData*>& VisualizerCanvas::getData() {
	return m_data;
}

void VisualizerCanvas::clearData() {
	m_data.clear();
	m_axis_descriptions.clear();
}

void VisualizerCanvas::addAxisDescription( string desc ) {
	m_axis_descriptions.push_back(desc);
}

const vector<string>& VisualizerCanvas::getAxisDescriptions() {
	return  m_axis_descriptions;
}