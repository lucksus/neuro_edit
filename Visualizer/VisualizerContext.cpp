#include "VisualizerContext.h"
#include "Visualizer.h"
#include <QMutexLocker>
#include <boost/foreach.hpp>
#include <assert.h>

VisualizerContext::VisualizerContext(void)
{
	m_backgroundColor[0] = m_backgroundColor[1] = m_backgroundColor[2] = 1.f;
}

VisualizerContext::~VisualizerContext(void)
{
	for(unsigned int i=0;i<m_data.size();++i)
		delete m_data[i];

	Visualizer::getInstance().deleteContext(this);
}


void VisualizerContext::setDescription(string description){
	QMutexLocker locker(&m_mutex);
	m_description = description;
}

string VisualizerContext::getDescription(){
	QMutexLocker locker(&m_mutex);
	return m_description;
}

void VisualizerContext::setBackgroundColor(float red, float green, float blue){
	QMutexLocker locker(&m_mutex);
	m_backgroundColor[0] = red;
	m_backgroundColor[1] = green;
	m_backgroundColor[2] = blue;
}

float VisualizerContext::getBackgroundRed(){
	return m_backgroundColor[0];
}

float VisualizerContext::getBackgroundGreen(){
	return m_backgroundColor[1];
}

float VisualizerContext::getBackgroundBlue(){
	return m_backgroundColor[2];
}

void VisualizerContext::clearData(){
	QMutexLocker locker(&m_mutex);

	BOOST_FOREACH(VisualizerData* v, m_data)
		delete v;

	m_data.clear();
}

void VisualizerContext::addData( VisualizerData* data ) {
	QMutexLocker locker(&m_mutex);
	m_data.push_back(data);
}

const vector<VisualizerData*>& VisualizerContext::getData(){
	QMutexLocker locker(&m_mutex);
	return m_data;
}

void VisualizerContext::addAxis(string name, string unit) {
	QMutexLocker locker(&m_mutex);
	Axis a(name, unit);
	m_axes.push_back(a);
}

const vector<VisualizerContext::Axis>& VisualizerContext::getAxes() {
	QMutexLocker locker(&m_mutex);
	return m_axes;
}

void VisualizerContext::updateAxis(unsigned int axis, VisualizerContext::Axis content) {
	QMutexLocker locker(&m_mutex);
	if (axis < m_axes.size()) {
		m_axes[axis] = content;
	} else assert(false);
}