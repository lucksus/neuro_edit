#pragma once
#include <vector>
#include "VisualizerData.h"
#include <complex>
using namespace std;

class VisualizerData2D : public VisualizerData
{
public:
	VisualizerData2D(void);
	VisualizerData2D(const VisualizerData2D& c);

	struct Point{
		double x;
		complex<double> y;
	};
	enum MODE{REAL,ABS,IMAG};

	void setData(vector<Point> data);
	void setData(vector<pair<double,double> > data);
	void resizeData(unsigned int);
	void writeData(unsigned int index, double x, complex<double> y);
	void setShowAbs();
	void setShowReal();
	void setShowImag();
	void setShowMode(MODE);
	MODE getShowMode();

	const vector<Point>& getData();
	const double value(unsigned int i) const;

	
private:
	vector<Point> m_data;
	MODE m_mode;
};
