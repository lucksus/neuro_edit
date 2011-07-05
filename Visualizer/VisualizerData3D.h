#pragma once
#include <vector>
#include <map>
#include <string>
#include "VisualizerData.h"

using namespace std;

class VisualizerData3D : public VisualizerData
{
public:
	VisualizerData3D(void);
	VisualizerData3D(const VisualizerData3D& c);

	struct Point{
		double x, y, z;
	};

	void setData(const vector<VisualizerData3D::Point>& data);
	const vector<VisualizerData3D::Point>& getData();
	void writeData(unsigned int index, double x, double y, double z);
	void resizeData(unsigned int size);

	void setBold(bool bold);
	bool isBold();

	void setAnnotation( const string& name, const vector<double>& annotation );
	bool hasAnnotation( const string& name );
	const vector<double>& getAnnotation( const string& name );

private:
	vector<Point> m_data;
	map< string, vector<double> > m_annotations;
	bool m_bold;
};
