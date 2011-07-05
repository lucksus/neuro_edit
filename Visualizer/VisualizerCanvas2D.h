#pragma once
#include "VisualizerCanvas.h"
#include <vector>
#include "VisualizerData2D.h"
#include <QFont>
#include <QString>

class VisualizerContext2D;

class VisualizerCanvas2D : public VisualizerCanvas
{
Q_OBJECT
public:
	VisualizerCanvas2D(QWidget* parent, VisualizerContext2D* context);
	virtual ~VisualizerCanvas2D(void);

	void addData(VisualizerData*);
	void clearData();
	void setNavigationMode();
	void setPickerMode();
	void setZoomMode();

	double getXMinOfVisualRange();
	double getXMaxOfVisualRange();
	double getYMinOfVisualRange();
	double getYMaxOfVisualRange();


public slots:
	void snapViewAroundData();
	void updateVisualizerData(VisualizerData*);

signals:
	void newMouseCoordinates(float x, float y);
	void newPickerCoordinates(float x1, float y1, float x2, float y2);
	void switchedToNavigationMode();

protected:
	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	
	virtual void paintGL();
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent* event);

private:
	VisualizerContext2D* m_context;
	enum Mode{NAVIGATION, PICKER, ZOOM};
	Mode m_mode;
	float m_xSizeForCross;
	float m_ySizeForCross;
	int m_xAxisExponent;
	int m_yAxisExponent;
	int m_axisExponentDiff;
	pair<float,float> m_data_x_min_max;
	pair<float,float> m_data_y_min_max;
	pair<float,float> m_currentOrigin;
	pair<float,float> m_currentRegion;

	pair<float,float> m_leftPicker;
	pair<float,float> m_rightPicker;
	pair<float,float> m_hoverPicker;

	unsigned int m_lastMouseX;
	unsigned int m_lastMouseY;
	unsigned int m_zoomMouseX;
	unsigned int m_zoomMouseY;

	map<VisualizerData2D*,GLuint> m_displayLists;
	map<VisualizerData2D*,GLuint> m_displayListsCross;

	void calculateCrossSize();
	void setProjectionMatrix();
	void createDisplayList(VisualizerData2D*);
	void deleteDisplayLists();
	void deleteDisplayListsWhichAreNoVisualizerData2DList();
	void calculateExponentsFromDataRegion();
	void calculateExponentsFromCurrentRegion();
	void drawGrid();
	void drawGridLinesX(vector<double>);
	void drawGridLinesY(vector<double>);
	void drawNumbers();
	unsigned int widthOfBackgroundFrameForYNumbers();
	unsigned int heightOfBackgroundFrameForXNumbers();
	vector<double> getXSteps(bool onlyFullSteps);
	vector<double> getYSteps(bool onlyFullSteps);
	void drawPicker(pair<float,float>, float*);
	void drawZoomRect(float x1, float y1, float x2, float y2);
	void renderTextWithBoundingBox(int x, int y, QString text, QFont font = QFont(), bool xcentered = false, bool ycentered = false);
	void drawNumbersBackgroundFrame();
	void calculateMaxMinValuesOverAllDatas();
	void adaptMaxMinValues(VisualizerData2D* data, bool considerAsFirstData = false);
	
	pair< pair<float,float>, pair<float,float> > zoomRegionWindowToValue(pair< pair<int,int>, pair<int,int> >);
	pair<unsigned int, unsigned int> valueToWindow(pair<float,float>);
	pair<float, float> windowToValue(pair<int,int>);
	pair<float,float> nearestDataPoint(pair<float,float>);
	pair<float,float> nearestWindowPoint(pair<float,float>);

};


