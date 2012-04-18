#include "VisualizerCanvasPoincare.h"
#include <QMouseEvent>
#include <assert.h>
#include "VisualizerContext3D.h"
#include <cmath>
#include <limits>
#include <boost/foreach.hpp>
#include <algorithm>

#define pi 3.14159256

double log(double,int){
    assert(false);
    return 0;
}

VisualizerCanvasPoincare::VisualizerCanvasPoincare(QWidget* parent, VisualizerContext3D* context)
: VisualizerCanvas(parent, context), m_context(context), m_sphere_model(0), m_data_models(0)
{
	setBackgroundColor(1.f, 1.f, 1.f);
	m_mode = NAVIGATION;
	snapViewAroundData();
}

VisualizerCanvasPoincare::~VisualizerCanvasPoincare(void)
{
	deleteModels();
}

void VisualizerCanvasPoincare::refresh() {
	deleteModels();
}

void VisualizerCanvasPoincare::setXAxisDescription(string desc) { m_xAxisDescription = desc; }
void VisualizerCanvasPoincare::setYAxisDescription(string desc) { m_yAxisDescription = desc; }
void VisualizerCanvasPoincare::setZAxisDescription(string desc) { m_zAxisDescription = desc; }

void VisualizerCanvasPoincare::addData(VisualizerData* data) {
	makeCurrent();

	VisualizerData3D* data_3d = dynamic_cast<VisualizerData3D*>( data );
	assert( data_3d );

	VisualizerCanvas::addData( data_3d );
}

void VisualizerCanvasPoincare::clearData() {
	deleteModels();
	VisualizerCanvas::clearData();
}

void VisualizerCanvasPoincare::initializeGL() {
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void VisualizerCanvasPoincare::resizeGL(int width, int height) {
	makeCurrent();

	glViewport(0,0, width, height);
	setViewport();
}

void VisualizerCanvasPoincare::paintGL() {
	clearCanvas();
	setViewport();
	paintSphere();
	paintData();
	paintAxisLabels();
}

void VisualizerCanvasPoincare::setViewport() {
	makeCurrent();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-m_viewport.width / 2.0f , m_viewport.width / 2.0f , 
			-m_viewport.height / 2.0f, m_viewport.height / 2.0f, 
			0.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef( m_translation.x, m_translation.y, -1.5f );
	glRotatef( m_rotation.x, 1.0f, 0.0f, 0.0f );
	glRotatef( m_rotation.y, 0.0f, 1.0f, 0.0f );

	// Kippen der gesamten Kugel um 90 Grad um s1, damit die Darstellung derjenigen von MATLAB
	// gleicht:
	glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
}

void VisualizerCanvasPoincare::clearCanvas() {
	makeCurrent();
	glClearColor(getBackgroundColor().red, getBackgroundColor().green, getBackgroundColor().blue, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void VisualizerCanvasPoincare::paintSphere() {
	makeCurrent();
	if (!m_sphere_model) m_sphere_model = createSphereModel();
	glCallList(m_sphere_model);
}

void VisualizerCanvasPoincare::paintData() {
	makeCurrent();

	if ( m_data_models.empty() ) {
		BOOST_FOREACH( VisualizerData* data, getData() ) {
			m_data_models.push_back( createDataModel( data ) );
		}
	}

	BOOST_FOREACH( GLuint model, m_data_models ) glCallList(model);
}

void VisualizerCanvasPoincare::paintAxisLabels() {
	glColor4f(0.0, 0.0, 0.0, 1.0);
	const double dist = 1.23f;
	const double off  = 0.02 * dist;

	QFont font;
	font.setPixelSize( 51.0f / m_viewport.height );

	renderText(dist,  off,  off, "s1", font);
	renderText( off, dist,  off, "s2", font);
	renderText( off,  off, dist, "s3", font);
}

GLuint VisualizerCanvasPoincare::createSphereModel() {
	makeCurrent();

	const double sphere_radius	= 1.0f;			// Radius der Kugel beträgt 1.
	const double delta_phi		= pi / 12.0;	// Kugel wird in 15-Grad-Schritten unterteilt.

	GLuint sphere = glGenLists(1);
	
	glNewList(sphere, GL_COMPILE);
	glEnable(GL_BLEND);
	
	glColor4f(	1-getBackgroundColor().red+0.6,
				1-getBackgroundColor().green+0.6,
				1-getBackgroundColor().blue+0.6,
				0.8f );
	glLineStipple(3, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	
	for ( double phi = delta_phi; phi < pi; phi += delta_phi ) {
		double z_pos  = sphere_radius * cos(phi);
		double radius = sphere_radius * sin(phi);
		drawCircle( Point(0.0f, 0.0f, z_pos), radius, Point(1.0f, 0.0f, 0.0f), 0.0f );
	}

	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	for ( double phi = 0; phi < pi; phi += delta_phi ) {
		drawCircle( Point(0.0f, 0.0, 0.0f), sphere_radius, Point(0.0f, 1.0f, 0.0f), phi / pi * 180.0 );
	}
	glPopMatrix();

	glDisable(GL_LINE_STIPPLE);

	// Zeichne die Achsen ein:
	drawAxes();

	glDisable(GL_BLEND);
	glEndList();

	return sphere;
}

void VisualizerCanvasPoincare::drawAxes() {
	drawAxis( Axis( Angle(  0.0,  0.0), RGBAColor(0.0, 0.0, 0.0, 1.0) ) );
	drawAxis( Axis( Angle(-90.0,  0.0), RGBAColor(0.0, 0.0, 0.0, 1.0) ) );
	drawAxis( Axis( Angle(  0.0, 90.0), RGBAColor(0.0, 0.0, 0.0, 1.0) ) );
}

void VisualizerCanvasPoincare::drawAxis( const Axis& a ) {
	makeCurrent();

	glPushMatrix();
	glRotatef(a.direction.theta, 0.0, 1.0, 0.0);
	glRotatef(a.direction.phi,   0.0, 0.0, 1.0);

	const double length = 1.18f;
	glColor4f(a.color.r, a.color.g, a.color.b, a.color.a);
	glBegin(GL_LINES);
		glVertex3f(  0.0f, 0.0f, 0.0f);
		glVertex3f(length, 0.0f, 0.0f);
	glEnd();

	glPushMatrix();
	glTranslatef(length, 0.0, 0.0);

	const double width = 0.03;
	glBegin(GL_TRIANGLES);
		glVertex3f(-width * 3, 0.0, -width);
		glVertex3f(-width * 3, 0.0,  width);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glRotatef(90, 1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
		glVertex3f(-width * 3, 0.0, -width);
		glVertex3f(-width * 3, 0.0,  width);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glPopMatrix();

	glPopMatrix();
}

GLuint VisualizerCanvasPoincare::createDataModel( VisualizerData* raw_data ) {
	makeCurrent();

	VisualizerData3D* data = dynamic_cast<VisualizerData3D*>( raw_data );
	assert( data );

	GLuint data_model = glGenLists(1);

	glNewList(data_model, GL_COMPILE);
	glEnable(GL_BLEND);

	if ( data->isVisible() ) {
		switch ( data->getStyle() ) {
			case VisualizerData::LINE: 
			case VisualizerData::DASHED_LINE:
			case VisualizerData::DASHED_DOTTED_LINE:
				drawDataAsLines(data); break;
			case VisualizerData::CROSS:
			case VisualizerData::SQUARE:
				drawDataAsCrosses(data); break;
			default: assert(false);
		}
	}
	
	glDisable(GL_BLEND);
	glEndList();

	return data_model;
}

vector<double> VisualizerCanvasPoincare::getAlphaValues( VisualizerData3D* data ) {
	vector<double> intensities;
	intensities.resize( data->getData().size(), 1.0f );
	
	if ( data->hasAnnotation("Intensity") && data->isTransparent() ) {
		intensities = data->getAnnotation("Intensity");
		double highest = -std::numeric_limits<double>::infinity();
		double lowest  =  std::numeric_limits<double>::infinity();
		for (uint i = 0; i < intensities.size(); ++i) {
			if ( intensities[i] > highest ) highest = intensities[i];
			if ( intensities[i] < lowest  ) lowest  = intensities[i];
		}

		lowest  = log(lowest + 1.0f , 10);
		highest = log(highest + 1.0f, 10);

		if ( (highest - lowest) / highest < 1e-6 ) {							// Im Falle, das alle Intensitäten (nahezu) gleich sind...
                        return vector<double>( data->getData().size(), 1.0f );	// Gleichverteilung zurückgeben, denn die Intensitäten sind ja auch gleichverteilt.
		}

		for (uint i = 0; i < intensities.size(); ++i) {
			intensities[i] = ( log( intensities[i] + 1.0f , 10 ) - lowest ) / (highest - lowest);
			if ( intensities[i] < 0.10 ) {
				intensities[i] = 0.0;
			}
		}
	}

	return intensities;
}

void VisualizerCanvasPoincare::drawDataAsLines( VisualizerData3D* data ) {
	if (data->getStyle() == VisualizerData::DASHED_LINE) {
		glLineStipple (1, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
	}
	if (data->getStyle() == VisualizerData::DASHED_DOTTED_LINE) {
		glLineStipple (1, 0x1C47);
		glEnable(GL_LINE_STIPPLE);
	}

	vector<double> alpha = getAlphaValues(data);

	glBegin( GL_LINE_STRIP );
	for (uint i = 0; i < data->getData().size(); ++i) {
		VisualizerData3D::Point p = data->getData()[i];
		glColor4f(data->getColorRed(),data->getColorGreen(),data->getColorBlue(),alpha[i]);
		glVertex3f( p.x, p.y, p.z );
	}
	glEnd();

	if ( glIsEnabled( GL_LINE_STIPPLE ) ) glDisable(GL_LINE_STIPPLE);
}

void VisualizerCanvasPoincare::drawDataAsCrosses( VisualizerData3D* data ) {
	const double cross = 3.0f / 100.0f;

	vector<double> alpha = getAlphaValues(data);

	if ( data->isBold() ) glLineWidth( 5.0f );

	for (uint i = 0; i < data->getData().size(); ++i) {
		VisualizerData3D::Point p = data->getData()[i];	
		glBegin( GL_LINES );
			glColor4f(data->getColorRed(),data->getColorGreen(),data->getColorBlue(),alpha[i]);

			glVertex3f( p.x - cross / 2.0, p.y, p.z );
			glVertex3f( p.x + cross / 2.0, p.y, p.z );

			glVertex3f( p.x, p.y - cross / 2.0f, p.z );
			glVertex3f( p.x, p.y + cross / 2.0f, p.z );

			glVertex3f( p.x, p.y, p.z - cross / 2.0f );
			glVertex3f( p.x, p.y, p.z + cross / 2.0f );
		glEnd();
	}

	if ( data->isBold() ) glLineWidth( 1.0f );
}

void VisualizerCanvasPoincare::drawCircle( Point center, double radius, Point rotationVector, double angle ) {
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	glRotatef(angle, rotationVector.x, rotationVector.y, rotationVector.z);

	const double delta_phi = 2*pi / 100;
	glBegin(GL_LINE_LOOP);
	for ( double phi = 0; phi < 2*pi; phi += delta_phi ) {
		glVertex3f( radius * cos(phi), radius * sin(phi), 0.0f );
	}
	glEnd();

	glPopMatrix();
}

void VisualizerCanvasPoincare::deleteModels() {
	makeCurrent();
	if (m_sphere_model) {
		glDeleteLists(m_sphere_model, 1);
		m_sphere_model = 0;
	}
	if (!m_data_models.empty()) {
		BOOST_FOREACH( GLuint model, m_data_models ) {
			glDeleteLists(model, 1);
		}
		m_data_models.clear();
	}
}

void VisualizerCanvasPoincare::mousePressEvent(QMouseEvent *event) {
	m_last_mouse_pos.first  = event->x();
	m_last_mouse_pos.second = event->y();
}

void VisualizerCanvasPoincare::mouseReleaseEvent(QMouseEvent *) {

}

void VisualizerCanvasPoincare::mouseMoveEvent(QMouseEvent *event) {
	pair<int, int> current( event->x(), event->y() );
	pair<int, int> last = m_last_mouse_pos;

	if (m_mode == NAVIGATION) {
		switch (event->buttons()) {
			case Qt::LeftButton: 
				rotate( current.first - last.first, current.second - last.second );
				break;
			case Qt::RightButton:
				translate( current.first - last.first, current.second - last.second );
				break;
		}
		updateGL();
	}	

	m_last_mouse_pos = current;
}

void VisualizerCanvasPoincare::wheelEvent(QWheelEvent* event) {
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	if (event->orientation() == Qt::Vertical) {
		if ( (m_viewport.width > 2 * numSteps / 10.0f) && (m_viewport.height > 2 * numSteps / 10.0f) ) {
			m_viewport.width  -= numSteps / 10.0f;
			m_viewport.height -= numSteps / 10.0f;
		}
		setViewport();

		updateGL();

		event->accept();
	}
}

void VisualizerCanvasPoincare::snapViewAroundData() {
	m_rotation.x = m_rotation.y = m_rotation.z = 0.0f;
	m_translation.x = m_translation.y = m_translation.z = 0.0f;
	m_viewport.width = m_viewport.height = 3.0f;

	setViewport();

	if ( isVisible() ) {
		makeCurrent();
		updateGL();
	}
}

void VisualizerCanvasPoincare::updateVisualizerData(VisualizerData*) {
	deleteModels();
	updateGL();
}

void VisualizerCanvasPoincare::rotate( int x, int y ) {
	const double px2deg = 0.5 / 3.0 * m_viewport.width;
	m_rotation.x += y * px2deg;	
	m_rotation.y += x * px2deg;

	if (m_rotation.x >  90.0f) m_rotation.x =  90.0f;
	if (m_rotation.x < -90.0f) m_rotation.x = -90.0f;
}

void VisualizerCanvasPoincare::translate( int x, int y ) {
	float diminisher = 600.0f / m_viewport.width;
	m_translation.x += x / diminisher;
	m_translation.y -= y / diminisher;
}
