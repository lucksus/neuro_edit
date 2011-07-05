#include "VisualizerData2D.h"

VisualizerData2D::VisualizerData2D(void)
{
}

VisualizerData2D::VisualizerData2D(const VisualizerData2D& c) 
	: VisualizerData(c), m_data(c.m_data), m_mode(c.m_mode)
{
}

void VisualizerData2D::setData(vector<Point> data){
	QMutexLocker locker(&m_mutex);
	m_data = data;
}

void VisualizerData2D::setData(vector<pair<double,double> > data){
	QMutexLocker locker(&m_mutex);
	m_data.resize(data.size());
	for(unsigned int i=0;i<data.size();++i){
		m_data[i].x = data[i].first;
		m_data[i].y = data[i].second;
	}
}

const vector<VisualizerData2D::Point>& VisualizerData2D::getData(){
	QMutexLocker locker(&m_mutex);
	return m_data;
}

//! Bring den zu plottenden Datenvektor schon einmal auf die korrekte Größe:
void VisualizerData2D::resizeData(unsigned int size){
	m_data.resize(size);
}
//! Daten in den Visualizer weg schreiben. Muss für jeden Index des zu plottenden
//! Arrays aufgerufen werden. "index" ist der aktuelle Index, in den geschrieben wird.
void VisualizerData2D::writeData(unsigned int index, double x, complex<double> y){
	m_data[index].x = x;
	m_data[index].y = y;
}

void VisualizerData2D::setShowAbs(){
	m_mode = ABS;
}

void VisualizerData2D::setShowReal(){
	m_mode = REAL;
}

void VisualizerData2D::setShowImag(){
	m_mode = IMAG;
}

void VisualizerData2D::setShowMode(VisualizerData2D::MODE mode){
	m_mode = mode;
}

VisualizerData2D::MODE VisualizerData2D::getShowMode(){
	return m_mode;
}

const double VisualizerData2D::value(unsigned int i) const{
	switch(m_mode){
		case REAL: return m_data[i].y.real();
		case ABS: return sqrt(pow(m_data[i].y.real(),2)+pow(m_data[i].y.imag(),2));
		case IMAG: return m_data[i].y.imag();
		default: return m_data[i].y.real();
	}
}