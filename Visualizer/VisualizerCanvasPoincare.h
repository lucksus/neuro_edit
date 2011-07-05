#pragma once

#include "VisualizerCanvas.h"
#include <vector>
#include "VisualizerData3D.h"

class VisualizerContext3D;

class VisualizerCanvasPoincare : public VisualizerCanvas
{
Q_OBJECT
public:
	VisualizerCanvasPoincare(QWidget* parent, VisualizerContext3D* context);
	virtual ~VisualizerCanvasPoincare(void);

	void addData(VisualizerData*);
	void clearData();

	void refresh();

	void setXAxisDescription(string);
	void setYAxisDescription(string);
	void setZAxisDescription(string);

	virtual void setNavigationMode() {}
	virtual void setPickerMode() {}
	virtual void setZoomMode() {}

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
	void clearCanvas();
	void paintSphere();
	void paintData();
	void paintAxisLabels();

	struct Point {
		Point(double X, double Y, double Z) : x(X), y(Y), z(Z) {}
		double x, y, z;
	};

	struct RGBAColor {
		RGBAColor(double a_r, double a_g, double a_b, double a_a) 
			: r(a_r), g(a_g), b(a_b), a(a_a) {}
		double r, g, b, a;
	};

	struct Angle {
		Angle(double a_theta, double a_phi) : theta(a_theta), phi(a_phi) {};
		double theta, phi;
	};

	struct Axis {
		Axis(Angle a_direction, RGBAColor a_color) 
			: direction(a_direction), color(a_color) {}
		Angle direction;
		RGBAColor color;
	};

	void drawCircle( Point center, double radius, Point rotationVector, double angle );
	void drawAxes();
	void drawAxis( const Axis& );
	void drawDataAsLines( VisualizerData3D* data );
	void drawDataAsCrosses( VisualizerData3D* data );
	vector<double> getAlphaValues( VisualizerData3D* data );

	pair<int, int> m_last_mouse_pos;

	void setViewport();

	GLuint createSphereModel();
	GLuint createDataModel( VisualizerData* raw_data );
	void deleteModels();

	void rotate( int x, int y );
	void translate( int x, int y );

	struct { float x, y, z; } m_rotation;
	struct { float x, y, z; } m_translation;
	struct { float width, height; } m_viewport;

	VisualizerContext3D* m_context;
	enum Mode{NAVIGATION, PICKER, ZOOM};
	string m_xAxisDescription;
	string m_yAxisDescription;
	string m_zAxisDescription;
	Mode m_mode;
	float m_xSizeForCross;
	float m_ySizeForCross;
	float m_backgroundColor[3];
	GLuint m_sphere_model;
	vector<GLuint> m_data_models;
};
