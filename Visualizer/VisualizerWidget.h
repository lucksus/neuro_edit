#ifndef VISUALIZERWIDGET_H
#define VISUALIZERWIDGET_H

#include <QMainWindow>
#include "ui_visualizerwidget.h"
class VisualizerContext;
class VisualizerContext2D;
class VisualizerContext3D;
class VisualizerCanvas;
class ObjectsTreeWidget;

class VisualizerWidget : public QMainWindow
{
	Q_OBJECT

public:
	VisualizerWidget(VisualizerContext* context, QWidget *parent = 0);
	~VisualizerWidget();

public slots:
	void updateFromContext();
	void updatePickerCoordinates(float x1, float y1, float x2, float y2);
	void show();
        void update();

protected:
	void keyReleaseEvent ( QKeyEvent * event );

private:
	Ui::VisualizerWidget ui;
	VisualizerCanvas* m_canvas;
	VisualizerContext* m_context;
	ObjectsTreeWidget* m_treeWidget;

	void initializeCanvasAndGUI(VisualizerContext*);

private slots:
	void on_actionNavigationsmodus_triggered(bool checked);
	void on_actionAuswahlmodus_triggered(bool checked);
	void on_actionZoom_triggered(bool checked);
	void on_actionTo_Ascii_triggered();
	void on_actionTo_Image_triggered();
	void showMouseCoordinatesInStatusBar(float x, float y);
	void canvasSwitchedToNavigationMode();

};

#endif // VISUALIZERWIDGET_H
