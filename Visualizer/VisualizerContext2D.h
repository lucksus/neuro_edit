#pragma once
#include "VisualizerContext.h"
#include "VisualizerData2D.h"

class VisualizerContext2D : public VisualizerContext
{
public:
	VisualizerContext2D(void);
	virtual ~VisualizerContext2D();
	void addData(VisualizerData2D*);

	void setFixedScalingRatio(bool b);
	bool isFixedScalingRatio();
	void setPredefinedRegion(double xmin, double ymin, double xsize, double ysize);
	bool hasPredefinedRegion();
	pair< pair<double,double>, pair<double,double> > getPredefinedRegion();
	
private:
	string m_y_name;
	string m_y_unit;
	pair< pair<double,double>, pair<double,double> > m_predefinedRegion;
	bool m_hasPredefinedRegion;
	bool m_fixedScalingRatio;
};
