#include "VisualizerData2DList.h"
#include <limits>

VisualizerData2DList::VisualizerData2DList(void)
: m_behaveLikeLabOscilloscope(true)
{
}

void VisualizerData2DList::setXAxisSize(unsigned int size){
	m_xvalues.resize(size);
	for(unsigned int i=0;i<m_yvalues.size();++i)
		m_yvalues[i].resize(m_xvalues.size(),numeric_limits<double>::signaling_NaN());
}

void VisualizerData2DList::setYValuesCount(unsigned int size){
	m_yvalues.resize(size);
	for(unsigned int i=0;i<size;++i)
		m_yvalues[i].resize(m_xvalues.size(),numeric_limits<double>::signaling_NaN());
}

void VisualizerData2DList::setXValue(unsigned int i, double d){
	m_xvalues[i] = d;
}

void VisualizerData2DList::setYValue(unsigned int i, unsigned int j, double d){
	m_yvalues[i][j] = d;
}

const vector<double>& VisualizerData2DList::getXValues(){
	return m_xvalues;
}

const vector< vector<double> >& VisualizerData2DList::getYValues(){
	return m_yvalues;
}

void VisualizerData2DList::setBehaveLikeLabOscilloscope(bool b){
	m_behaveLikeLabOscilloscope = b;
}

bool VisualizerData2DList::behaveLikeLabOscilloscope(){
	return m_behaveLikeLabOscilloscope;
}