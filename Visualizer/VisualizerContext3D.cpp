#include "VisualizerContext3D.h"
#include <QMutexLocker>
#include <boost/foreach.hpp>

VisualizerContext3D::VisualizerContext3D(void)
: VisualizerContext()
{
}

void VisualizerContext3D::addData(VisualizerData3D* data) {
	VisualizerContext::addData( data );
}

void VisualizerContext3D::setYName(string name) { m_y_name = name; }
string VisualizerContext3D::getYName() { return m_y_name; }
void VisualizerContext3D::setYUnit(string unit) { m_y_unit = unit; }
string VisualizerContext3D::getYUnit() { return m_y_unit; }

void VisualizerContext3D::setZName(string name) { m_z_name = name; }
string VisualizerContext3D::getZName() { return m_z_name; }
void VisualizerContext3D::setZUnit(string unit) { m_z_unit = unit; }
string VisualizerContext3D::getZUnit() { return m_z_unit; }