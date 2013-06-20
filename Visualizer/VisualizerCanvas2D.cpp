#include "VisualizerCanvas2D.h"
#include "VisualizerContext2D.h"
#include <vector>
#include <assert.h>
#include <math.h>
#include <sstream>
#include <QMouseEvent>
#include <algorithm>
#include "VisualizerData2DList.h"
#include <QFontMetrics>
#include <limits>
#include <boost/foreach.hpp>
#include "VisualizerContext2D.h"
#ifdef Q_OS_WIN
#include <gl/glu.h>
#endif
#ifdef Q_OS_MACX
#include <glu.h>
#endif

#define _isnan(x) (x!=x)

bool isZero(vector<double> numbers, unsigned int i){
	double step=numbers[numbers.size()-1]-numbers[numbers.size()-2];
	return (numbers[i]-(step/2))*(numbers[i]+(step/2)) < 0.f;
}

VisualizerCanvas2D::VisualizerCanvas2D(QWidget* parent, VisualizerContext2D* context)
: VisualizerCanvas(parent, context), m_context(context), m_data_x_min_max(0.f,0.f), m_data_y_min_max(0.f,0.f), m_currentOrigin(0.f,0.f), m_currentRegion(1.f,1.f)
{
	setBackgroundColor(1.f, 1.f, 1.f);
	setMouseTracking(true);
	setCursor(Qt::SizeAllCursor);
	m_mode = NAVIGATION;
}

VisualizerCanvas2D::~VisualizerCanvas2D(void)
{
	makeCurrent();
	clearData();
}

void VisualizerCanvas2D::addData(VisualizerData* data){
	makeCurrent();

	VisualizerData2D* data_2d = dynamic_cast<VisualizerData2D*>( data );
	assert( data_2d );
	VisualizerCanvas::addData(data_2d);

	snapViewAroundData();
}

void VisualizerCanvas2D::calculateMaxMinValuesOverAllDatas(){
	if(getData().size()<=0)return;
	adaptMaxMinValues( dynamic_cast<VisualizerData2D*>(getData()[0]),true );
	BOOST_FOREACH(VisualizerData* data, getData()){
		adaptMaxMinValues( dynamic_cast<VisualizerData2D*>(data) );
	}
}

void VisualizerCanvas2D::adaptMaxMinValues(VisualizerData2D* data, bool considerAsFirstData){
	bool firstData;
	if(considerAsFirstData) firstData = true;
	else firstData = getData().size() == 0;

	if(!data->isVisible()) return;

	float minx=0,maxx=0,miny=0,maxy=0;
	bool firstTime = true;
	VisualizerData2DList* dataList;
        if((dataList = dynamic_cast<VisualizerData2DList*>(data))){
		vector<double> xvalues = dataList->getXValues();
		vector<vector<double> > yvalues = dataList->getYValues();
		for(unsigned int n=0;n<yvalues.size();++n)
			for(unsigned int i=0;i<xvalues.size();++i){
				if(firstTime){
					if(xvalues[i] != std::numeric_limits<double>::quiet_NaN()) minx=maxx=xvalues[i];
					if(yvalues[n][i] != std::numeric_limits<double>::quiet_NaN()) miny=maxy=yvalues[n][i];
					firstTime=false;
				}else{
					if(xvalues[i] < minx && minx != std::numeric_limits<double>::quiet_NaN()) minx = xvalues[i];
					if(xvalues[i] > maxx && maxx != std::numeric_limits<double>::quiet_NaN()) maxx = xvalues[i];
					if(yvalues[n][i] < miny && miny != std::numeric_limits<double>::quiet_NaN()) miny = yvalues[n][i];
					if(yvalues[n][i] > maxy && maxy != std::numeric_limits<double>::quiet_NaN()) maxy = yvalues[n][i];
				}
			}
	}else{
		vector<VisualizerData2D::Point> points = data->getData();
		for(unsigned int i=0;i<points.size();++i){
			if(firstTime){
				if(points[i].x != std::numeric_limits<double>::quiet_NaN()) minx=maxx=points[i].x;
				if(data->value(i) != std::numeric_limits<double>::quiet_NaN()) miny=maxy=data->value(i);
				firstTime=false;
			}else{
				if(points[i].x < minx && minx != std::numeric_limits<double>::quiet_NaN()) minx = points[i].x;
				if(points[i].x > maxx && maxx != std::numeric_limits<double>::quiet_NaN()) maxx = points[i].x;
				if(data->value(i) < miny && miny != std::numeric_limits<double>::quiet_NaN()) miny = data->value(i);
				if(data->value(i) > maxy && maxy != std::numeric_limits<double>::quiet_NaN()) maxy = data->value(i);
			}
		}
	}

	if(minx < m_data_x_min_max.first || firstData)m_data_x_min_max.first=minx;
	if(maxx > m_data_x_min_max.second || firstData)m_data_x_min_max.second=maxx;
	if(miny < m_data_y_min_max.first || firstData)m_data_y_min_max.first=miny;
	if(maxy > m_data_y_min_max.second || firstData)m_data_y_min_max.second=maxy;

	if(_isnan(m_data_x_min_max.first) || m_data_x_min_max.first == std::numeric_limits<double>::infinity()
		|| m_data_x_min_max.first == -std::numeric_limits<double>::infinity())
		m_data_x_min_max.first = 0;

	if(_isnan(m_data_x_min_max.second) || m_data_x_min_max.second == std::numeric_limits<double>::infinity()
		|| m_data_x_min_max.second == -std::numeric_limits<double>::infinity())
		m_data_x_min_max.second = 1;

	if(_isnan(m_data_y_min_max.first) || m_data_y_min_max.first == std::numeric_limits<double>::infinity()
		|| m_data_y_min_max.first == -std::numeric_limits<double>::infinity())
		m_data_y_min_max.first = 0;

	if(_isnan(m_data_y_min_max.second) || m_data_y_min_max.second == std::numeric_limits<double>::infinity()
		|| m_data_y_min_max.second == -std::numeric_limits<double>::infinity())
		m_data_y_min_max.second = 1;
}

void VisualizerCanvas2D::createDisplayList(VisualizerData2D* data){
	makeCurrent();
	GLuint newList = glGenLists (1);
	VisualizerData2DList* dataList;
        if((dataList = dynamic_cast<VisualizerData2DList*>(data))){
		vector<double> xvalues = dataList->getXValues();
		vector< vector<double> > yvalues = dataList->getYValues();
		
		glNewList(newList, GL_COMPILE);
		glEnable(GL_BLEND);
		if(dataList->behaveLikeLabOscilloscope())
			glColor4f(data->getColorRed(),data->getColorGreen(),data->getColorBlue(), 1.f/(5.f*log10(static_cast<float>(yvalues.size()))));
		else
			glColor4f(data->getColorRed(),data->getColorGreen(),data->getColorBlue(), 1.f);
		switch(data->getStyle()){
		case VisualizerData::LINE:
			glLineWidth(2.f);
			for(unsigned int n=0;n<yvalues.size();++n){
				glBegin(GL_LINES);
				for(unsigned int i=1;i<xvalues.size();++i){
					glVertex2d(xvalues[i-1],yvalues[n][i-1]);
					glVertex2d(xvalues[i],yvalues[n][i]);
				}
				glEnd();
			}
			
			glDisable(GL_BLEND);
			glEndList();
			m_displayLists[data]  = newList;
			glLineWidth(1.f);
			break;

		case VisualizerData::CROSS:
			glBegin(GL_LINES);
			for(unsigned int n=0;n<yvalues.size();++n)
				for(unsigned int i=0;i<xvalues.size();++i){
					glVertex2d(xvalues[i] - m_xSizeForCross/2,yvalues[n][i]);
					glVertex2d(xvalues[i] + m_xSizeForCross/2,yvalues[n][i]);
					glVertex2d(xvalues[i],yvalues[n][i] - m_ySizeForCross/2);
					glVertex2d(xvalues[i],yvalues[n][i] + m_ySizeForCross/2);
				}
			glEnd();
			glDisable(GL_BLEND);
			glEndList();
			m_displayListsCross[data]  = newList;
			break;

		default:
			assert(false);
		}
	}
	else{
		vector<VisualizerData2D::Point> points = data->getData();
		if(points.size()==0) return;

		glNewList(newList, GL_COMPILE);
		glColor3f(data->getColorRed(),data->getColorGreen(),data->getColorBlue());

		switch(data->getStyle()){
		case VisualizerData::LINE:
			glLineWidth(2.f);
			glBegin(GL_LINES);
			for(unsigned int i=1;i<points.size();++i){
				glVertex2d(points[i-1].x,data->value(i-1));
				glVertex2d(points[i].x,data->value(i));
			}
			glEnd();
			glLineWidth(1.f);
			glEndList();
			m_displayLists[data]  = newList;
		break;

		case VisualizerData::CROSS:
			glBegin(GL_LINES);
			for(unsigned int i=0;i<points.size();++i){
				glVertex2d(points[i].x - m_xSizeForCross/2,data->value(i));
				glVertex2d(points[i].x + m_xSizeForCross/2,data->value(i));
				glVertex2d(points[i].x,data->value(i) - m_ySizeForCross/2);
				glVertex2d(points[i].x,data->value(i) + m_ySizeForCross/2);
			}
			glEnd();
			glEndList();
			m_displayListsCross[data]  = newList;
		break;

		case VisualizerData::DASHED_DOTTED_LINE:
			glLineStipple (1, 0x1C47);
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
			for(unsigned int i=1;i<points.size();++i){
				glVertex2d(points[i-1].x,data->value(i-1));
				glVertex2d(points[i].x,data->value(i));
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			glEndList();
			m_displayListsCross[data]  = newList;
		break;

		case VisualizerData::DASHED_LINE:
			glLineStipple (1, 0xAAAA);
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
			for(unsigned int i=1;i<points.size();++i){
				glVertex2d(points[i-1].x,data->value(i-1));
				glVertex2d(points[i].x,data->value(i));
			}
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			glEndList();
			m_displayListsCross[data]  = newList;
		break;

		case VisualizerData::SQUARE:
			glLineWidth(2.f);
			glBegin(GL_LINES);
			for(unsigned int i=0;i<points.size();++i){
				glVertex2d(points[i].x - m_xSizeForCross/2,data->value(i) - m_ySizeForCross/2);
				glVertex2d(points[i].x + m_xSizeForCross/2,data->value(i) - m_ySizeForCross/2);

				glVertex2d(points[i].x + m_xSizeForCross/2,data->value(i) - m_ySizeForCross/2);
				glVertex2d(points[i].x + m_xSizeForCross/2,data->value(i) + m_ySizeForCross/2);

				glVertex2d(points[i].x + m_xSizeForCross/2,data->value(i) + m_ySizeForCross/2);
				glVertex2d(points[i].x - m_xSizeForCross/2,data->value(i) + m_ySizeForCross/2);

				glVertex2d(points[i].x - m_xSizeForCross/2,data->value(i) + m_ySizeForCross/2);
				glVertex2d(points[i].x - m_xSizeForCross/2,data->value(i) - m_ySizeForCross/2);
			}
			glEnd();
			glEndList();
			m_displayListsCross[data]  = newList;
			break;

		default:
			assert(false);
		}
	}
}


void VisualizerCanvas2D::initializeGL(){
	calculateCrossSize();
}

void VisualizerCanvas2D::resizeGL(int width, int height){
	glViewport(0,0, width, height);
	calculateCrossSize();
	deleteDisplayLists();
}

void VisualizerCanvas2D::paintGL(){
	calculateCrossSize();
	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	setProjectionMatrix();
	glClearColor(getBackgroundColor().red,getBackgroundColor().green,getBackgroundColor().blue,1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	//erst nur die Linien Zeichnen:
	drawGrid();

	//dann die Daten:
	for(unsigned int i=0;i<getData().size();++i){
		VisualizerData2D* container = dynamic_cast<VisualizerData2D*>(getData()[i]);
		assert( container );

		if(!container->isVisible())continue;
		if((m_displayLists.count(container) == 0) &&(m_displayListsCross.count(container) == 0))
			createDisplayList(container);
		if(m_displayLists.count(container)) glCallList(m_displayLists[container]);
		if(m_displayListsCross.count(container)) glCallList(m_displayListsCross[container]);
	}

	drawNumbersBackgroundFrame();
	//jetzt die Achsenbeschriftungen:
	drawNumbers();

        if(m_context->showTime())
            drawTime();



	if(m_mode == PICKER){
		float red[4]={1.f,0.f,0.f,1.0f};
		float green[4]={0.f,0.7f,0.f,1.0f};
		float yellow[4]={1.f,1.f,0.f,0.8f};
		drawPicker(m_leftPicker,red);
		drawPicker(m_rightPicker,green);
		drawPicker(m_hoverPicker,yellow);
	}

	if(m_mode == ZOOM){
		pair< pair<int,int>, pair<int,int> > windowPoints(pair<int,int>(m_zoomMouseX, m_zoomMouseY), pair<int,int>(m_lastMouseX,m_lastMouseY));
		pair< pair<float,float>, pair<float,float> > valuePoints = zoomRegionWindowToValue(windowPoints);
		drawZoomRect(valuePoints.first.first, valuePoints.first.second, valuePoints.second.first, valuePoints.second.second);
	}

	glColor4f(1-getBackgroundColor().red,1-getBackgroundColor().green,1-getBackgroundColor().blue,1.f);

	vector<string> axes = getAxisDescriptions();
	if ( axes.size() == 2 ) {
		renderTextWithBoundingBox(5,10,axes[1].c_str());
		renderTextWithBoundingBox(width()-6*axes[0].size(),height()-5,axes[0].c_str());
	}
}

unsigned int VisualizerCanvas2D::widthOfBackgroundFrameForYNumbers(){
	vector<double> yNumbers = getYSteps(true);
	unsigned int width=0;
	for(unsigned int i=0;i<yNumbers.size();++i){
		if(isZero(yNumbers,i)) yNumbers[i] = 0;
		stringstream s;
		s << yNumbers[i];
		QFont font;
		QFontMetrics m(font);
		QRect rect = m.boundingRect(s.str().c_str());
		if(rect.width() > static_cast<int>(width)) width = rect.width();
	}
	return width+2;
}

unsigned int VisualizerCanvas2D::heightOfBackgroundFrameForXNumbers(){
	vector<double> xNumbers = getXSteps(true);
	unsigned int height=0;
	for(unsigned int i=0;i<xNumbers.size();++i){
		if(isZero(xNumbers,i)) xNumbers[i] = 0;
		stringstream s;
		s << xNumbers[i];
		QFont font;
		QFontMetrics m(font);
		QRect rect = m.boundingRect(s.str().c_str());
		if(rect.height() > static_cast<int>(height)) height= rect.height();
	}
	return height*2.5;
}

void VisualizerCanvas2D::drawNumbersBackgroundFrame(){
	
	unsigned int width = widthOfBackgroundFrameForYNumbers();
	unsigned int height = heightOfBackgroundFrameForXNumbers();
	pair<unsigned int, unsigned int> widthWindow(pair<unsigned int,unsigned int>(width,0));
	pair<unsigned int, unsigned int> heightWindow(pair<unsigned int,unsigned int>(0,this->height()-height));
	pair<double,double> widthValue = windowToValue(widthWindow);
	pair<double,double> heightValue = windowToValue(heightWindow);
	widthValue.first -= m_currentOrigin.first;
	heightValue.second -= m_currentOrigin.second;

	glColor4f(getBackgroundColor().red,getBackgroundColor().green,getBackgroundColor().blue,1);
	glBegin(GL_QUADS);
		glVertex2d(m_currentOrigin.first, m_currentOrigin.second);
		glVertex2d(m_currentOrigin.first, m_currentOrigin.second + heightValue.second);
		glVertex2d(m_currentOrigin.first+m_currentRegion.first, m_currentOrigin.second + heightValue.second);
		glVertex2d(m_currentOrigin.first+m_currentRegion.first, m_currentOrigin.second);

		glVertex2d(m_currentOrigin.first, m_currentOrigin.second);
		glVertex2d(m_currentOrigin.first + widthValue.first, m_currentOrigin.second);
		glVertex2d(m_currentOrigin.first + widthValue.first, m_currentOrigin.second + m_currentRegion.second);
		glVertex2d(m_currentOrigin.first, m_currentOrigin.second + m_currentRegion.second);
	glEnd();

}

void VisualizerCanvas2D::mousePressEvent(QMouseEvent *event){
	if(m_mode == ZOOM){
		m_zoomMouseX = event->x();
		m_zoomMouseY = event->y();
	}
	if(m_mode == NAVIGATION){
		m_lastMouseX = event->x();
		m_lastMouseY = event->y();
	}

	if(m_mode == PICKER){
		pair<int,int> mouse;
		mouse.first = event->x();
		mouse.second = event->y();

		if(event->button() == Qt::LeftButton){
			m_leftPicker = nearestWindowPoint(mouse);
			emit newPickerCoordinates(m_leftPicker.first, m_leftPicker.second, m_rightPicker.first, m_rightPicker.second);
		}

		if(event->button() == Qt::RightButton){
			m_rightPicker = nearestWindowPoint(mouse);
			emit newPickerCoordinates(m_leftPicker.first, m_leftPicker.second, m_rightPicker.first, m_rightPicker.second);
		}
	}
}

void VisualizerCanvas2D::mouseReleaseEvent(QMouseEvent*){
	if(m_mode == ZOOM){
		m_mode = NAVIGATION;
		emit switchedToNavigationMode();
		
		pair< pair<int,int>, pair<int,int> > windowPoints(pair<int,int>(m_lastMouseX,m_lastMouseY), pair<int,int>(m_zoomMouseX,m_zoomMouseY));
		pair< pair<float,float>, pair<float,float> > valuePoints = zoomRegionWindowToValue(windowPoints);
		
		if(valuePoints.first.first == valuePoints.second.first || valuePoints.first.second == valuePoints.second.second) return;

		double xRatio = widthOfBackgroundFrameForYNumbers() / static_cast<double>(width());
		double yRatio = heightOfBackgroundFrameForXNumbers() / static_cast<double>(height());

		m_currentOrigin = valuePoints.first;
		m_currentRegion.first = valuePoints.second.first - valuePoints.first.first;
		m_currentRegion.second = valuePoints.second.second - valuePoints.first.second;

		//Anpassung, die wegen den Rändern für die Zahlen nötig ist:
		m_currentOrigin.first -= m_currentRegion.first*xRatio;
		m_currentOrigin.second -= m_currentRegion.second*yRatio;
		m_currentRegion.first += m_currentRegion.first*xRatio;
		m_currentRegion.second += m_currentRegion.second*yRatio;


		deleteDisplayListsWhichAreNoVisualizerData2DList();
		calculateExponentsFromCurrentRegion();

		updateGL();
	}
}

pair< pair<float,float>, pair<float,float> > VisualizerCanvas2D::zoomRegionWindowToValue(pair< pair<int,int>, pair<int,int> > p){
	int x1,x2,y1,y2;
	if(p.first.first > p.second.first){
		x1 = p.second.first;
		x2 = p.first.first;
	}else{
		x2 = p.second.first;
		x1 = p.first.first;
	}
	if(p.first.second < p.second.second){
		y1 = p.second.second;
		y2 = p.first.second;
	}else{
		y2 = p.second.second;
		y1 = p.first.second;
	}

	pair<int,int> bottomLeft, topRight;
	bottomLeft.first = x1;
	bottomLeft.second = y1;
	topRight.first = x2;
	topRight.second = y2;

	pair<float,float> valueBottomLeft, valueTopRight;
	valueBottomLeft = windowToValue(bottomLeft);
	valueTopRight = windowToValue(topRight);
	return pair< pair<float,float>, pair<float,float> >(valueBottomLeft,valueTopRight);
}

void VisualizerCanvas2D::mouseMoveEvent(QMouseEvent *event){
	pair<int, int> last, current;
	last.first = m_lastMouseX;
	last.second = m_lastMouseY;
	current.first = event->x();
	current.second = event->y();
	pair<float,float> lastValue, currentValue;
	lastValue = windowToValue(last);
	currentValue = windowToValue(current);

	m_lastMouseX = event->x();
	m_lastMouseY = event->y();

	emit newMouseCoordinates(currentValue.first,currentValue.second);

	if((m_mode == NAVIGATION) && (event->buttons() != Qt::NoButton)){
		m_currentOrigin.first -= currentValue.first - lastValue.first;
		m_currentOrigin.second -= currentValue.second - lastValue.second;
		updateGL();
	}
	
	if(m_mode == PICKER){
		m_hoverPicker = nearestWindowPoint(current);
		updateGL();
	}

	if((m_mode == ZOOM) && (event->buttons() != Qt::NoButton)){
		updateGL();
	}


	
	
}

void VisualizerCanvas2D::wheelEvent(QWheelEvent* event){
	if(m_mode != NAVIGATION) return;

	float factor = event->delta() > 0 ? 0.9f : 1.1f;
	pair<float,float> value = windowToValue(pair<float,float>(event->x(),event->y()));
	
	m_currentRegion.first *= factor;
	m_currentRegion.second *= factor;

	pair<float,float> value2 = windowToValue(pair<float,float>(event->x(),event->y()));

	m_currentOrigin.first -= value2.first - value.first;
	m_currentOrigin.second -= value2.second - value.second;//m_currentRegion.second/2;
	calculateCrossSize();
	calculateExponentsFromCurrentRegion();
	deleteDisplayListsWhichAreNoVisualizerData2DList();
	updateGL();
}


void VisualizerCanvas2D::setProjectionMatrix(){
	GLdouble left = m_currentOrigin.first;
	GLdouble right = left + m_currentRegion.first;
	GLdouble bottom = m_currentOrigin.second;
	GLdouble top = bottom + m_currentRegion.second;

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(left, right, bottom, top);
}

void VisualizerCanvas2D::clearData(){
	deleteDisplayLists();
	VisualizerCanvas::clearData();
}

void VisualizerCanvas2D::deleteDisplayLists(){
	makeCurrent();
	map<VisualizerData2D*,GLuint>::iterator it;
	for(it = m_displayLists.begin();it != m_displayLists.end();++it)
		glDeleteLists(it->second, 1);
	m_displayLists.clear();

	deleteDisplayListsWhichAreNoVisualizerData2DList();
}

void VisualizerCanvas2D::deleteDisplayListsWhichAreNoVisualizerData2DList(){
	makeCurrent();
	map<VisualizerData2D*,GLuint>::iterator it;
	for(it = m_displayListsCross.begin();it != m_displayListsCross.end();++it)
		glDeleteLists(it->second, 1);
	m_displayListsCross.clear();
}

void VisualizerCanvas2D::calculateExponentsFromDataRegion(){
	double x_region = m_data_x_min_max.second - m_data_x_min_max.first;
	double y_region = m_data_y_min_max.second - m_data_y_min_max.first;
	m_xAxisExponent = floor(log10(x_region));
	m_yAxisExponent = floor(log10(y_region));
	m_axisExponentDiff = m_yAxisExponent - m_xAxisExponent;
}

void VisualizerCanvas2D::calculateExponentsFromCurrentRegion(){
	double x_region = m_currentRegion.first*0.7;
	double y_region = m_currentRegion.second*0.7;
	m_xAxisExponent = floor(log10(x_region));
	m_yAxisExponent = floor(log10(y_region));
	//m_yAxisExponent = m_xAxisExponent + m_axisExponentDiff;
	m_axisExponentDiff = m_yAxisExponent - m_xAxisExponent;
}

vector<double> VisualizerCanvas2D::getXSteps(bool onlyFullSteps){
	vector<double> result;
	double x_pos = floor(m_currentOrigin.first/pow((double)10,(double)m_xAxisExponent)) * pow((double)10,(double)m_xAxisExponent);
	bool full=true;
	float xstep = pow((double)10,(double)m_xAxisExponent)/2;
	while(x_pos <= m_currentOrigin.first + m_currentRegion.first){
		if(onlyFullSteps){
			if(full) result.push_back(x_pos);
		}else
			result.push_back(x_pos);

		x_pos += xstep;
		full = !full;
	}
	return result;
}

vector<double> VisualizerCanvas2D::getYSteps(bool onlyFullSteps){
	vector<double> result;
	double y_pos = floor(m_currentOrigin.second/pow((double)10,(double)m_yAxisExponent)) * pow((double)10,(double)m_yAxisExponent);
	bool full=true;
	float ystep = pow((double)10,(double)m_yAxisExponent)/2;
	while(y_pos <= m_currentOrigin.second + m_currentRegion.second){
		if(onlyFullSteps){
			if(full) result.push_back(y_pos);
		}else
			result.push_back(y_pos);

		y_pos += ystep;
		full = !full;
	}
	return result;
}

void VisualizerCanvas2D::drawGridLinesX(vector<double> numbers){
	for(unsigned int i=0;i<numbers.size();++i){
		if(isZero(numbers,i)) glLineWidth(2);
		else glLineWidth(1);

		glColor4f(1-getBackgroundColor().red+0.6,1-getBackgroundColor().green+0.6,1-getBackgroundColor().blue+0.6,1.0f);
		glBegin(GL_LINES);
		glVertex2d(numbers[i],m_currentOrigin.second);
		glVertex2d(numbers[i],(m_currentOrigin.second*1000.0 + m_currentRegion.second*1000.0)/1000.0);
		glEnd();
	}
}

void VisualizerCanvas2D::drawGridLinesY(vector<double> numbers){
	for(unsigned int i=0;i<numbers.size();++i){
		if(isZero(numbers,i)) glLineWidth(2);
		else glLineWidth(1);

		glColor4f(1-getBackgroundColor().red+0.6,1-getBackgroundColor().green+0.6,1-getBackgroundColor().blue+0.6,1.0f);
		glBegin(GL_LINES);
		glVertex2d(m_currentOrigin.first, numbers[i]);
		glVertex2d((m_currentOrigin.first*1000.0 + m_currentRegion.first*1000.0)/1000.0, numbers[i]);
		glEnd();
	}
}

void VisualizerCanvas2D::drawGrid(){
	glLineStipple(3, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	drawGridLinesX(getXSteps(false));
	drawGridLinesY(getYSteps(false));
	glDisable(GL_LINE_STIPPLE);
	drawGridLinesX(getXSteps(true));
	drawGridLinesY(getYSteps(true));
}


void VisualizerCanvas2D::drawNumbers(){
	vector<double> xNumbers = getXSteps(m_currentRegion.first / pow(static_cast<double>(10),m_xAxisExponent) > 5);
	for(unsigned int i=0;i<xNumbers.size();++i){
		QFont font;
		if(isZero(xNumbers,i)){
			xNumbers[i] = 0;
			font.setBold(true);
		}
		stringstream s;
		s << xNumbers[i];
		pair<unsigned int, unsigned int> windowPos = valueToWindow(pair<float,float>(xNumbers[i], m_currentOrigin.second));
		if((windowPos.first > static_cast<unsigned int>(widthOfBackgroundFrameForYNumbers())) && (windowPos.first < static_cast<unsigned int>(width()))){
			renderTextWithBoundingBox(static_cast<int>(windowPos.first),height()-15,s.str().c_str(),font,true,false);
		}
	}

	vector<double> yNumbers = getYSteps(m_currentRegion.second / pow(static_cast<double>(10),m_yAxisExponent) > 5);
	for(unsigned int i=0;i<yNumbers.size();++i){
		QFont font;
		if(isZero(yNumbers,i)){
			yNumbers[i] = 0;
			font.setBold(true);
		}
		stringstream s;
		s << yNumbers[i];
		pair<unsigned int, unsigned int> windowPos = valueToWindow(pair<float,float>(m_currentOrigin.first, yNumbers[i]));
		if(static_cast<int>(windowPos.second) < (height()-30) && static_cast<int>(windowPos.second) > heightOfBackgroundFrameForXNumbers()/2){
			renderTextWithBoundingBox(5,windowPos.second,s.str().c_str(),font,false,true);
		}
	}
}


void VisualizerCanvas2D::renderTextWithBoundingBox(int x, int y, QString text, QFont font, bool xcentered, bool ycentered){
	QFontMetrics metrics(font);
	QRect boundingRect = metrics.boundingRect(text);
	if(xcentered) x -= boundingRect.width()/2;
	if(ycentered) y += boundingRect.height()/2;
	
	pair<unsigned int,unsigned int> lowerRightVertexOfBoundingRect;
	pair<unsigned int,unsigned int> upperLeftVertexOfBoundingRect;
	lowerRightVertexOfBoundingRect.first = x + boundingRect.width()+1;
	lowerRightVertexOfBoundingRect.second = y - (boundingRect.height()-4);
	upperLeftVertexOfBoundingRect.first = x-1;
	upperLeftVertexOfBoundingRect.second = y;
	pair<float,float> valueOfUpperLeftVertexOfBoundingRect = windowToValue(upperLeftVertexOfBoundingRect);
	pair<float,float> valueOfLowerRightVertexOfBoundingRect = windowToValue(lowerRightVertexOfBoundingRect);
	glColor3f(getBackgroundColor().red, getBackgroundColor().green, getBackgroundColor().blue);
	glBegin(GL_QUADS);
	glVertex2f(valueOfUpperLeftVertexOfBoundingRect.first, valueOfUpperLeftVertexOfBoundingRect.second);
	glVertex2f(valueOfLowerRightVertexOfBoundingRect.first, valueOfUpperLeftVertexOfBoundingRect.second);
	glVertex2f(valueOfLowerRightVertexOfBoundingRect.first, valueOfLowerRightVertexOfBoundingRect.second);
	glVertex2f(valueOfUpperLeftVertexOfBoundingRect.first, valueOfLowerRightVertexOfBoundingRect.second);
	glEnd();

	glColor4f(1-getBackgroundColor().red,1-getBackgroundColor().green,1-getBackgroundColor().blue,0.7f);
	renderText(x,y,text,font);
};

void VisualizerCanvas2D::snapViewAroundData(){
	makeCurrent();
	//calculateExponentsFromDataRegion();
	calculateMaxMinValuesOverAllDatas();

	if(m_context->hasPredefinedRegion()){
		m_currentOrigin = m_context->getPredefinedRegion().first;
		m_currentRegion = m_context->getPredefinedRegion().second;
	}else{
		m_currentOrigin.first = m_data_x_min_max.first;
		m_currentRegion.first = m_data_x_min_max.second - m_data_x_min_max.first;

		if(m_context->isFixedScalingRatio()){
			double ymean;
			double max = m_data_y_min_max.second;
			double min = m_data_y_min_max.first;
			assert(max >= min);
			ymean = (max + min)/2;
			
			m_currentOrigin.second = ymean - m_currentRegion.first/2;
			m_currentRegion.second = m_currentRegion.first;
		}else{
			m_currentOrigin.second = m_data_y_min_max.first;
			m_currentRegion.second = m_data_y_min_max.second - m_data_y_min_max.first;
		}

		if(m_currentRegion.first <= 0) m_currentRegion.first = 1.f;
		if(m_currentRegion.second <= 0) m_currentRegion.second = 1.f;
	}

	

	calculateExponentsFromCurrentRegion();

	m_currentOrigin.first -= pow((double)10,(double)m_xAxisExponent)*2;
	m_currentOrigin.second -= pow((double)10,(double)m_yAxisExponent)*2;
	m_currentRegion.first += pow((double)10,(double)m_xAxisExponent)*3;
	m_currentRegion.second += pow((double)10,(double)m_yAxisExponent)*3;
	
/*
	calculateExponentsFromCurrentRegion();
	double bx = pow((double)10,(double)m_xAxisExponent);
	double by = pow((double)10,(double)m_yAxisExponent);
	double x1 = floor(m_data_x_min_max.first/bx) * bx;
	double x2 = ceil(m_data_x_min_max.second/bx) * bx;
	double y1 = floor(m_data_y_min_max.first/by) * by;
	double y2 = ceil(m_data_y_min_max.second/by) * by;
	m_currentOrigin.first = x1-bx/2;
	m_currentOrigin.second = y1-by/2;
	m_currentRegion.first = x2-x1+bx;
	m_currentRegion.second = y2-y1+by;
*/
	deleteDisplayListsWhichAreNoVisualizerData2DList();
	updateGL();
}


pair<unsigned int, unsigned int> VisualizerCanvas2D::valueToWindow(pair<float,float> value){
	float left = m_currentOrigin.first;
	float right = left + m_currentRegion.first;
	float bottom = m_currentOrigin.second;
	float top = bottom + m_currentRegion.second;

	pair<unsigned int, unsigned int> result;
	result.first  = static_cast<unsigned int>((value.first - left) / (right - left) * width());
	result.second = static_cast<unsigned int>((1-(value.second - bottom) / (top - bottom)) * height());

	return result;
}

pair<float, float> VisualizerCanvas2D::windowToValue(pair<int,int> window){
	double left = m_currentOrigin.first;
	double right = left + m_currentRegion.first;
	double bottom = m_currentOrigin.second;
	double top = bottom + m_currentRegion.second;

	pair<float, float> result;
	result.first  = left + float(window.first) / width() * (right - left);
	result.second = bottom + (1-(float(window.second) / height())) * (top - bottom);

	if(result.first <= left)
		result.first = left;
	if(result.first >= right)
		result.first = right;
	if(result.second <= bottom)
		result.second = bottom;
	if(result.second >= top)
		result.second = top;

	return result;
}
void VisualizerCanvas2D::calculateCrossSize()
{
	float xsize = m_currentRegion.first;
	float ysize = m_currentRegion.second;
	m_xSizeForCross = 11*xsize/width();
	m_ySizeForCross = 11*ysize/height();
}

void VisualizerCanvas2D::setNavigationMode(){
	setCursor(Qt::SizeAllCursor);
	m_mode = NAVIGATION;
	updateGL();
}

void VisualizerCanvas2D::setPickerMode(){
	setCursor(Qt::CrossCursor);
	m_mode = PICKER;
	updateGL();
}

void VisualizerCanvas2D::setZoomMode(){
	setCursor(Qt::CustomCursor);
	m_mode = ZOOM;
	m_zoomMouseX = m_lastMouseX;
	m_zoomMouseY = m_lastMouseY;
	updateGL();
}


pair<float,float> VisualizerCanvas2D::nearestDataPoint(pair<float,float> ref){
	float min;
	pair<float,float> result ;
	bool firstTime = true;
	for(unsigned int d=0;d<getData().size();++d){
		VisualizerData2D* container = dynamic_cast<VisualizerData2D*>(getData()[d]);
		assert( container );
		vector<VisualizerData2D::Point> data = container->getData();
		for(unsigned int i=0;i<data.size();++i){
			VisualizerData2D::Point p = data[i];
			float diff = sqrt(pow(p.x-ref.first,2) + pow(p.y.real()-ref.second,2));
			if(firstTime || diff < min){
				firstTime = false;
				min = diff;
				result.first = p.x;
				result.second = p.y.real();
			}
		}
	}

	return result;
}

pair<float,float> VisualizerCanvas2D::nearestWindowPoint(pair<float,float> ref){
	float min;
	pair<float,float> result ;
	bool firstTime = true;
	for(unsigned int d=0;d<getData().size();++d){
		if(VisualizerData2DList* data2DList = dynamic_cast<VisualizerData2DList*>(getData()[d])){
			vector<double> xvalues = data2DList->getXValues();
			vector< vector<double> > yvalues = data2DList->getYValues();
			for(unsigned int x=0;x<xvalues.size();++x)
				for(unsigned int y=0;y<yvalues.size();++y){
					std::pair<float,float> dataPoint;
					dataPoint.first = xvalues[x];
					dataPoint.second = yvalues[y][x];
					dataPoint = valueToWindow(dataPoint);
					float diff = sqrt(pow(dataPoint.first-ref.first,2) + pow(dataPoint.second-ref.second,2));
					if(firstTime || diff < min){
						firstTime = false;
						min = diff;
						result.first = xvalues[x];
						result.second = yvalues[y][x];
					}
				}
			continue;
		}

		VisualizerData2D* container = dynamic_cast<VisualizerData2D*>(getData()[d]);
		assert( container );
		vector<VisualizerData2D::Point> data = container->getData();
		for(unsigned int i=0;i<data.size();++i){
			VisualizerData2D::Point p = data[i];
			std::pair<float,float> dataPoint;
			dataPoint.first = p.x;
			dataPoint.second = container->value(i);
			dataPoint = valueToWindow(dataPoint);
			float diff = sqrt(pow(dataPoint.first-ref.first,2) + pow(dataPoint.second-ref.second,2));
			if(firstTime || diff < min){
				firstTime = false;
				min = diff;
				result.first = p.x;
				result.second = p.y.real();
			}
		}
	}

	return result;
}

void VisualizerCanvas2D::drawPicker(pair<float,float> pos, float* color){
	glColor4fv(color);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2d(pos.first, m_currentOrigin.second);
		glVertex2d(pos.first, m_currentOrigin.second + m_currentRegion.second);
		glVertex2d(m_currentOrigin.first, pos.second);
		glVertex2d(m_currentOrigin.first + m_currentRegion.first, pos.second);
	glEnd();
}

void VisualizerCanvas2D::drawZoomRect(float x1, float y1, float x2, float y2){
	glColor4f(1.f,1.f,0.f,1.f);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x1,y2);

		glVertex2f(x1,y2);
		glVertex2f(x2,y2);

		glVertex2f(x2,y2);
		glVertex2f(x2,y1);

		glVertex2f(x2,y1);
		glVertex2f(x1,y1);
	glEnd();

}

void VisualizerCanvas2D::updateVisualizerData(VisualizerData* data){
	VisualizerData2D* data2D = dynamic_cast<VisualizerData2D*>(data);
	assert(data2D);
	makeCurrent();
	if(m_displayLists.count(data2D)){
		glDeleteLists(m_displayLists[data2D],1);
		m_displayLists.erase(m_displayLists.find(data2D));
	}
	if(m_displayListsCross.count(data2D)){
		glDeleteLists(m_displayListsCross[data2D],1);
		m_displayListsCross.erase(m_displayListsCross.find(data2D));
	}
	updateGL();
}


double VisualizerCanvas2D::getXMinOfVisualRange(){
	return m_currentOrigin.first;
}

double VisualizerCanvas2D::getXMaxOfVisualRange(){
	return m_currentOrigin.first + m_currentRegion.first;
}

double VisualizerCanvas2D::getYMinOfVisualRange(){
	return m_currentOrigin.second;
}

double VisualizerCanvas2D::getYMaxOfVisualRange(){
	return m_currentOrigin.second + m_currentRegion.second;
}

void VisualizerCanvas2D::drawTime(){
    glColor4fv(m_context->getTimeColor());
    glBegin(GL_LINES);
    glVertex2d(m_context->getTime(), m_currentOrigin.second);
    glVertex2d(m_context->getTime(), m_currentOrigin.second + m_currentRegion.second);
    glEnd();
}
