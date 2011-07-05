#include "VisualizerCanvas3D.h"
#include <QMouseEvent>
#include "VisualizerContext3D.h"

VisualizerCanvas3D::VisualizerCanvas3D(QWidget* parent, VisualizerContext3D* context)
: QGLWidget(parent), m_context(context)
{
	m_backgroundColor[0] = m_backgroundColor[1] = m_backgroundColor[2] = 1.f;
	m_mode = NAVIGATION;
}

VisualizerCanvas3D::~VisualizerCanvas3D(void)
{
}

void VisualizerCanvas3D::setBackgroundColor(float red, float green, float blue) {
	m_backgroundColor[0] = red;
	m_backgroundColor[1] = green;
	m_backgroundColor[2] = blue;
}

void VisualizerCanvas3D::addData(VisualizerData3D*) {

}

void VisualizerCanvas3D::clearData() {

}

void VisualizerCanvas3D::initializeGL() {

}

void VisualizerCanvas3D::resizeGL(int width, int height) {

}

void VisualizerCanvas3D::paintGL() {

}

void VisualizerCanvas3D::mousePressEvent(QMouseEvent *event) {

}

void VisualizerCanvas3D::mouseReleaseEvent(QMouseEvent *event) {

}

void VisualizerCanvas3D::mouseMoveEvent(QMouseEvent *event) {

}

void VisualizerCanvas3D::wheelEvent(QWheelEvent* event) {

}

void VisualizerCanvas3D::snapViewAroundData() {

}

void VisualizerCanvas3D::updateVisualizerData(VisualizerData*) {

}