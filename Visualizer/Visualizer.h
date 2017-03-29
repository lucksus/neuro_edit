#ifndef VISUALIZER_H
#define VISUALIZER_H
#include <map>
#include <QObject>
#include <QMutex>
#include "VisualizerContext.h"
#include "VisualizerContext2D.h"
#include "VisualizerContext3D.h"
#include "VisualizerWidget.h"
#include <QObject>

using namespace std;

class  Visualizer
{
friend class VisualizerContext;
public:
	static Visualizer& getInstance();

	VisualizerContext2D* createContext2D();
	VisualizerContext3D* createContext3D();
	void showWindowFor(VisualizerContext*);
	void updateWindowFor(VisualizerContext*);
	void hideWindowFor(VisualizerContext*);

	void deleteWidgets();

        void setTime(double);

protected:
	void deleteContext(VisualizerContext*);

private:
	Visualizer();
	~Visualizer();

	QMutex m_mutex;

	map<VisualizerContext*, VisualizerWidget*> m_widgets;

};

#define CREATE_VISUALIZER_CONTEXT_3D(context,description)			\
{																	\
	if (!context)													\
	{																\
		context = Visualizer::getInstance().createContext3D();		\
		context->setDescription( get_name() + " (" + get_identifier() + "), " + description );	\
	}																\
}

/*#define CREATE_VISUALIZER_CONTEXT_POINCARE(context,description)			\
{																	\
	if (!context)													\
{																\
	context = Visualizer::getInstance().createContextPoincare();		\
	context->setDescription( get_name() + " (" + get_identifier() + "), " + description );	\
}																\
}*/

#define CREATE_VISUALIZER_CONTEXT_2D(context,xname,yname,xunit,yunit,description) 	\
{ \
	if(!context) \
{ \
	context = Visualizer::getInstance().createContext2D(); \
	context->addAxis(xname, xunit); \
	context->addAxis(yname, yunit); \
	string tex1 = get_name(); \
	tex1 += " ("; \
	tex1 += get_identifier(); \
	tex1 += "), "; \
	tex1 += description; \
	context->setDescription(tex1); \
} \
} 

#define CREATE_VISUALIZER_CONTEXT_2D_NON_COMPONENT(context,xname,yname,xunit,yunit,description) 	\
{ \
	if(!context) \
{ \
	context = Visualizer::getInstance().createContext2D(); \
	context->addAxis(xname, xunit); \
	context->addAxis(yname, yunit); \
	context->setDescription(description); \
} \
} 

#endif // VISUALIZER_H
