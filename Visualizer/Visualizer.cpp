#include "Visualizer.h"
#include <QMutexLocker>
#include <QResource>


Visualizer::Visualizer()
{
	static bool initialized = false;
	if(!initialized) {
		Q_INIT_RESOURCE(Visualizer);
		initialized = true;
	}
}

Visualizer::~Visualizer()
{
	//Jenachdem woher benutzt, gibt es verschiedene (also mehrere) Visualizer Objekte..
	//Das aus der Pfadanalyse hat Probleme, mit dem Aufruf von deleteWidgets().
	//Offensichtlich ist zu dem Zeitpunkt, an dem dieser Destruktor aufgerufen wird,
	//irgendetwas nicht mehr so, wie Qt-Fenster oder OpenGL es erwarten..
	//Die einfachste Lösung ist, dies hier auszukommentieren.
	//Das dadurch entstehende Speicherleck ist nicht schlimm, da wir hier eh nur landen,
	//wenn PHOTOSS gerade beendet wird..
	//deleteWidgets();
}

void Visualizer::deleteWidgets(){
	map<VisualizerContext*,VisualizerWidget*>::iterator it;
	for(it=m_widgets.begin();it!=m_widgets.end();++it)
		delete it->second;
	m_widgets.clear();
}

Visualizer& Visualizer::getInstance(){
	static Visualizer v;
	return v;
}

VisualizerContext2D* Visualizer::createContext2D(){
	QMutexLocker locker(&m_mutex);
	VisualizerContext2D* context = new VisualizerContext2D();
	m_widgets[context] = new VisualizerWidget(context);
	return context;
}	

VisualizerContext3D* Visualizer::createContext3D() {
	QMutexLocker locker(&m_mutex);
	VisualizerContext3D* context = new VisualizerContext3D();
	m_widgets[context] = new VisualizerWidget(context);
	return context;
}
	
void Visualizer::showWindowFor(VisualizerContext* context){
	QMutexLocker locker(&m_mutex);
	updateWindowFor(context);
	m_widgets[context]->show();
}

void Visualizer::updateWindowFor(VisualizerContext* context){
	VisualizerWidget* widget = m_widgets[context];
	widget->setWindowTitle(context->getDescription().c_str());
	widget->updateFromContext();
}

void Visualizer::hideWindowFor(VisualizerContext* context){
	QMutexLocker locker(&m_mutex);
	m_widgets[context]->hide();
}

void Visualizer::deleteContext(VisualizerContext* context){
	QMutexLocker locker(&m_mutex);
	delete m_widgets[context];
	m_widgets.erase(m_widgets.find(context));
}
