#include "VisualizerContext2D.h"
#include <QMutexLocker>
#include <boost/foreach.hpp>

VisualizerContext2D::VisualizerContext2D(void)
: m_hasPredefinedRegion(false), m_fixedScalingRatio(false)
{
}

VisualizerContext2D::~VisualizerContext2D(){
}

void VisualizerContext2D::addData(VisualizerData2D* data){
	VisualizerContext::addData(data);
}

void VisualizerContext2D::setFixedScalingRatio(bool b){
	m_fixedScalingRatio = b;
}

bool VisualizerContext2D::isFixedScalingRatio(){
	return m_fixedScalingRatio;
}

void VisualizerContext2D::setPredefinedRegion(double xmin, double ymin, double xsize, double ysize){
	QMutexLocker locker(&m_mutex);
	m_predefinedRegion = pair< pair<double,double>, pair<double,double> >(
		pair<double,double>(xmin,ymin),
		pair<double,double>(xsize,ysize)
		);
	m_hasPredefinedRegion = true;
}

bool VisualizerContext2D::hasPredefinedRegion(){
	QMutexLocker locker(&m_mutex);
	return m_hasPredefinedRegion;
}

pair< pair<double,double>, pair<double,double> > VisualizerContext2D::getPredefinedRegion(){
	QMutexLocker locker(&m_mutex);
	return m_predefinedRegion;
}