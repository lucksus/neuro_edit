#ifndef GLPLOTWIDGET2D_H
#define GLPLOTWIDGET2D_H

#include <QGLWidget>
#include <string>
#include <list>

using namespace std;

class GLPlotWidget2d : public QGLWidget{
Q_OBJECT
public:
    GLPlotWidget2d(QWidget* parent = 0);

public slots:
    void set_region(double min_x, double min_y, double max_x, double max_y);
    void set_number_of_x_lines(unsigned int);
    void set_number_of_y_lines(unsigned int);
    void set_x_unit(string);
    void set_y_unit(string);

    void add_x_line_at(double pos, bool stipple);
    void add_y_line_at(double pos, bool stipple);

    void clear_lines();


protected:
     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();

     struct Point2DWithAlpha{
         double x,y,alpha;
     };

     virtual list<Point2DWithAlpha> data() = 0;


private:
    void setup_projection_matrix();
    void paint_axis();
    void paint_values();

    double m_min_x, m_min_y, m_max_x, m_max_y;
    string m_x_unit, m_y_unit;
    list< pair<double,bool> > m_x_lines;
    list< pair<double,bool> > m_y_lines;
    unsigned int m_number_of_x_lines;
    unsigned int m_number_of_y_lines;

};

#endif // GLPLOTWIDGET2D_H
