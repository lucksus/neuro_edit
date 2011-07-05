#pragma once

#include <QtOpenGL/QGLWidget>
#include <vector>
#include "VisualizerData3D.h"

class VisualizerContext3D;

class VisualizerCanvas3D : public QGLWidget
{
Q_OBJECT
public:
	VisualizerCanvas3D(QWidget* parent, VisualizerContext3D* context);
	virtual ~VisualizerCanvas3D(void);

	void setBackgroundColor(float red, float green, float blue);
	void addData(VisualizerData3D*);
	void clearData();

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
	VisualizerContext3D* m_context;
	enum Mode{NAVIGATION, PICKER, ZOOM};
	string m_xAxisDescription;
	string m_yAxisDescription;
	string m_zAxisDescription;
	Mode m_mode;
	float m_xSizeForCross;
	float m_ySizeForCross;
	float m_backgroundColor[3];
};
