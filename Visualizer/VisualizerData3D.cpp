#include "VisualizerData3D.h"
#include <assert.h>

VisualizerData3D::VisualizerData3D(void)
: m_bold(true)
{
}

VisualizerData3D::VisualizerData3D(const VisualizerData3D& c) 
: m_data(c.m_data), m_bold(c.m_bold)
{
}

void VisualizerData3D::setData(const vector<VisualizerData3D::Point>& data) {
	QMutexLocker locker(&m_mutex);
	m_data = data;
}

const vector<VisualizerData3D::Point>& VisualizerData3D::getData(){
	QMutexLocker locker(&m_mutex);
	return m_data;
}

void VisualizerData3D::resizeData(unsigned int size){
	m_data.resize(size);
}

void VisualizerData3D::writeData(unsigned int index, double x, double y, double z){
	m_data[index].x = x;
	m_data[index].y = y;
	m_data[index].z = z;
}

void VisualizerData3D::setAnnotation( const string& name, const vector<double>& annotation ) {
	QMutexLocker locker(&m_mutex);
	m_annotations[name] = annotation;
}

bool VisualizerData3D::hasAnnotation( const string& name ) {
	return m_annotations.find(name) != m_annotations.end();
}

bool VisualizerData3D::isBold() {
	return m_bold;
}

void VisualizerData3D::setBold(bool bold) {
	m_bold = bold;
}

const vector<double>& VisualizerData3D::getAnnotation( const string& name ) {
	QMutexLocker locker(&m_mutex);
	assert( m_annotations.find(name) != m_annotations.end() );
	return m_annotations[name];
}