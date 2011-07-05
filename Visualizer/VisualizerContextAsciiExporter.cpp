#include "VisualizerContextAsciiExporter.h"
#include <QFileDialog>
#include "VisualizerContext2D.h"
#include "VisualizerData2DList.h"
#include <assert.h>
#include <QIcon>

VisualizerContextAsciiExporter::VisualizerContextAsciiExporter(VisualizerContext* context)
:m_context(context)
{
}


void VisualizerContextAsciiExporter::doExport(){
	VisualizerContext2D* context = dynamic_cast<VisualizerContext2D*>(m_context);
	assert(context);
	QString qfileName = QFileDialog::getSaveFileName(0, "Export Visualizer to Ascii",
		"",
		QString("Textfile (*.txt);;")); 
	if(qfileName.isEmpty()) return;

	string fileName = qfileName.toStdString();
	fileName = fileName.substr(0, fileName.find_last_of("."));

	// Die einzelnen Object nach Matlab übertragen
	for (unsigned int i = 0; i < context->getData().size(); i++)
	{
		VisualizerData* data = context->getData()[i];
		if(!data->isVisible()) continue;
		string currentFileName = fileName + "_" + data->getName() + ".txt";
		ofstream file(currentFileName.c_str());
		
		if(VisualizerData2DList* data2DList = dynamic_cast<VisualizerData2DList*>(data))
			toAscii(data2DList,file);
		else
			toAscii(dynamic_cast<VisualizerData2D*>(data),file);
	}
}


void VisualizerContextAsciiExporter::toAscii(VisualizerData2D *data, std::ostream &out){
	out << "%x-value\ty-value" << endl;

	int oldprecision = out.precision(16);
	for (unsigned int i=0; i<data->getData().size(); i++)
		out << data->getData()[i].x << "\t" << data->value(i) << endl;

	out.precision(oldprecision);
}


void VisualizerContextAsciiExporter::toAscii(VisualizerData2DList *data, std::ostream &out){
	int oldprecision = out.precision(16);

	unsigned int l;
	// Zuerst mal die Titelzeile schreiben
	out << "%x-value\t";
	for (l=0; l<data->getYValues().size(); l++)
		out << "y-value[" << l << "]\t";

	out << endl;

	// und jetzt die Werte Ausgeben
	for (unsigned int t=0; t<data->getXValues().size(); t++)
	{
		out << data->getXValues()[t] << "\t";
		for (l=0; l<data->getYValues().size(); l++)
		{
			if (t>=data->getYValues()[l].size()) continue;
			out << data->getYValues()[l][t] << "\t";
		}

		out << endl;
	}
	out.precision(oldprecision);
}