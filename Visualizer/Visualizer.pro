TARGET = Visualizer
TEMPLATE = lib

CONFIG += staticlib qt
CONFIG -= flat

QT += core gui


DEPENDPATH += . Resources
INCLUDEPATH += . \
			..\gpc_lib\source \
			..\gpc_lib\core \
			..\photoss\photoss_dll\source \
			..\3rdparty\boost



# Input
HEADERS += BoolEditor.h \
           ColorListEditor.h \
           ObjectsTreeWidget.h \
           OscilloscopeBehaviourEditor.h \
           ShowModeEditor.h \
           StyleEditor.h \
		   TransparencyEditor.h \
		   BoldEditor.h \
           VisibleEditor.h \
           Visualizer.h \
		   VisualizerCanvas.h \
           VisualizerCanvas2D.h \
		   VisualizerCanvasPoincare.h \
           VisualizerContext.h \
           VisualizerContext2D.h \
		   VisualizerContext3D.h \
           VisualizerContextAsciiExporter.h \
           VisualizerData.h \
           VisualizerData2D.h \
           VisualizerData2DList.h \
           VisualizerData3D.h \
           VisualizerWidget.h
FORMS += VisualizerWidget.ui
SOURCES += BoolEditor.cpp \
           ColorListEditor.cpp \
           ObjectsTreeWidget.cpp \
           OscilloscopeBehaviourEditor.cpp \
           ShowModeEditor.cpp \
           StyleEditor.cpp \
		   TransparencyEditor.cpp \
		   BoldEditor.cpp \
           VisibleEditor.cpp \
           Visualizer.cpp \
		   VisualizerCanvas.cpp \
           VisualizerCanvas2D.cpp \
		   VisualizerCanvasPoincare.cpp \
           VisualizerContext.cpp \
           VisualizerContext2D.cpp \
		   VisualizerContext3D.cpp \
           VisualizerContextAsciiExporter.cpp \
           VisualizerData.cpp \
           VisualizerData2D.cpp \
           VisualizerData2DList.cpp \
           VisualizerData3D.cpp \
           VisualizerWidget.cpp
RESOURCES += Resources/Visualizer.qrc
