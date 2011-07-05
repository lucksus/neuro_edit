#pragma once
#include "VisualizerData2D.h"

class VisualizerData2DList : public VisualizerData2D
{
public:
	VisualizerData2DList(void);

	void setXAxisSize(unsigned int);
	void setYValuesCount(unsigned int);
	void setXValue(unsigned int, double);
	void setYValue(unsigned int, unsigned int, double);
	void setBehaveLikeLabOscilloscope(bool b);
	bool behaveLikeLabOscilloscope();

	const vector<double>& getXValues();
	const vector< vector<double> >& getYValues();


private:
	vector<double> m_xvalues;
	vector< vector<double> > m_yvalues;
	bool m_behaveLikeLabOscilloscope;
};
