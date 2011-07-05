#pragma once
#include <fstream>
using namespace std;
class VisualizerContext;
class VisualizerData2D;
class VisualizerData2DList;

class VisualizerContextAsciiExporter
{
public:
	VisualizerContextAsciiExporter(VisualizerContext*);
	void doExport();
private:
	VisualizerContext* m_context;
	void toAscii(VisualizerData2D*, ostream&);
	void toAscii(VisualizerData2DList*, ostream&);
};
