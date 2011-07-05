#pragma once

#include <QtOpenGL/QGLWidget>
#include <vector>

using namespace std;

class VisualizerContext;
class VisualizerData;

class VisualizerCanvas : public QGLWidget
{
public:
	VisualizerCanvas(QWidget* parent, VisualizerContext* context);
	virtual ~VisualizerCanvas(void);

	struct RGBColor {
		float red, green, blue;
	};

	void setBackgroundColor(float red, float green, float blue);
	RGBColor getBackgroundColor();

	virtual void setNavigationMode() = 0;
	virtual void setPickerMode() = 0;
	virtual void setZoomMode() = 0;

	virtual void addData( VisualizerData* );
	virtual void clearData();

	virtual void refresh() {};

	virtual void addAxisDescription( string desc );
	virtual const vector<string>& getAxisDescriptions();

protected:
	const vector<VisualizerData*>& getData();

private:
	RGBColor m_backgroundColor;
	vector<VisualizerData*> m_data;
	vector<string> m_axis_descriptions;
};
