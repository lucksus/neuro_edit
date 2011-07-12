#pragma once
#include <string>
#include <QMutex>
#include <vector>
using namespace std;

class VisualizerData;

class VisualizerContext
{
public:
	VisualizerContext(void);
	virtual ~VisualizerContext(void);

	void setDescription(string description);
	string getDescription();
	
	struct Axis {
		Axis( string a_name, string a_unit ) : name(a_name), unit(a_unit) {}

		string name;
		string unit;
	};
	void addAxis(string name, string unit);
	const vector<Axis>& getAxes();
	void updateAxis(unsigned int axis, Axis content);
	
	void  setBackgroundColor(float red, float green, float blue);
	float getBackgroundRed();
	float getBackgroundGreen();
	float getBackgroundBlue();

        void setShowTime(bool);
        bool showTime();
        void setTimeColor(float red, float green, float blue, float alpha);
        float* getTimeColor();

	virtual void clearData();
	virtual const vector<VisualizerData*>& getData();


        void setTime(double);
        double getTime();

protected:
	void addData( VisualizerData* );

	QMutex m_mutex;

private:
	string m_description;
	string m_x_name;
	string m_x_unit;
	float m_backgroundColor[3];
        float m_timeColor[4];
	vector<VisualizerData*> m_data;
	vector<Axis> m_axes;
        double m_time;
        bool m_show_time;
};
