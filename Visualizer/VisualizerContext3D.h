#pragma once
#include <vector>
#include "VisualizerData3D.h"
#include "VisualizerContext.h"

class VisualizerContext3D : public VisualizerContext {
public:
	VisualizerContext3D();

	// Routinen, die Datenreihen hinzufügen und löschen:
	void addData(VisualizerData3D*);

	void setYName(string);
	string getYName();
	void setYUnit(string);
	string getYUnit();

	void setZName(string);
	string getZName();
	void setZUnit(string);
	string getZUnit();

private:
	string m_y_name;
	string m_y_unit;
	string m_z_name;
	string m_z_unit;
};