#include "glplotwidget2d.h"
#include <boost/foreach.hpp>
#ifdef Q_OS_WIN
#include <gl/glu.h>
#endif
#ifdef Q_OS_MACX
#include <glu.h>
#endif

GLPlotWidget2d::GLPlotWidget2d(QWidget *parent) :
    QGLWidget(parent),
    m_number_of_x_lines(0),
    m_number_of_y_lines(0)
{
}

void GLPlotWidget2d::set_region(double min_x, double min_y, double max_x, double max_y){
    m_min_x = min_x;
    m_min_y = min_y;
    m_max_x = max_x;
    m_max_y = max_y;
}

void GLPlotWidget2d::set_number_of_x_lines(unsigned int n){
    m_number_of_x_lines = n;
}

void GLPlotWidget2d::set_number_of_y_lines(unsigned int n){
    m_number_of_y_lines = n;
}

void GLPlotWidget2d::set_x_unit(string u){
    m_x_unit = u;
}

void GLPlotWidget2d::set_y_unit(string u){
    m_y_unit = u;
}

void GLPlotWidget2d::add_x_line_at(double pos, bool stipple){
    m_x_lines.push_back(pair<double,bool>(pos, stipple));
}

void GLPlotWidget2d::add_y_line_at(double pos, bool stipple){
    m_y_lines.push_back(pair<double,bool>(pos, stipple));
}



void GLPlotWidget2d::initializeGL(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLPlotWidget2d::resizeGL(int w, int h){
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void GLPlotWidget2d::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!isVisible()) return;
    setup_projection_matrix();
    paint_axis();
    paint_values();
    glFlush();
}


void GLPlotWidget2d::setup_projection_matrix(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-110, -110, 120, 120);
    gluOrtho2D(m_min_x, m_max_x, m_min_y, m_max_y);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //double size_x = m_max_x-m_min_x;
    //double size_y = m_max_y-m_min_y;
    //double center_x = (m_max_x+m_min_x)/2;
    //double center_y = (m_max_y+m_min_y)/2;

    //glScalef(10/size_x,10/size_y,1.);
    //glTranslatef(-center_x,-center_y,0);

    //glScalef(1/50.,1/70.,1.);
    //glTranslatef(50,35,0);
}

void GLPlotWidget2d::paint_axis(){
    glColor3f(.7,.7,.7);
    glLineStipple (1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glDisable(GL_LINE_SMOOTH);

    glLineWidth(1);
    glBegin(GL_LINES);

    if(0 != m_number_of_x_lines){
        double step = (m_max_x-m_min_x)/m_number_of_x_lines;
        for(double x = m_min_x; x <= m_max_x; x += step){
            glVertex2f(x,m_min_y); glVertex2f(x,m_max_y);
        }
    }

    if(0 != m_number_of_y_lines){
        double step = (m_max_y-m_min_y)/m_number_of_y_lines;
        for(double y = m_min_y; y <= m_max_x; y += step){
            glVertex2f(m_min_x,y); glVertex2f(m_max_x,y);
        }
    }

    glEnd();

    pair<double,bool> line;
    BOOST_FOREACH(line, m_x_lines){
        if(line.second)
            glEnable(GL_LINE_STIPPLE);
        else
            glDisable(GL_LINE_STIPPLE);
        glBegin(GL_LINES);
        glVertex2f(line.first,m_min_y); glVertex2f(line.first,m_max_y);
        glEnd();
    }
    BOOST_FOREACH(line, m_y_lines){
        if(line.second)
            glEnable(GL_LINE_STIPPLE);
        else
            glDisable(GL_LINE_STIPPLE);
        glBegin(GL_LINES);
        glVertex2f(m_min_x,line.first); glVertex2f(m_max_x,line.first);
        glEnd();
    }




    QFont font;
    QFontMetrics metrics(font);

    glColor3f(1.,1.,1.);

    BOOST_FOREACH(line, m_x_lines){
        double x = line.first;
        QString text = QString("%1%2").arg(x).arg(m_x_unit.c_str());
        double new_x = (x-m_min_x)/(m_max_x-m_min_x)*width() - metrics.boundingRect(text).width()/2;
        renderText(new_x, height(), text);
    }

    BOOST_FOREACH(line, m_y_lines){
        double y = line.first;
        QString text = QString("%1%2").arg(y).arg(m_y_unit.c_str());
        double new_y = (y-m_min_y)/(m_max_y-m_min_y)*height() - metrics.boundingRect(text).height()/2;
        renderText(0, height() - new_y, text);
    }

}

void GLPlotWidget2d::paint_values(){
    bool first = true;
    glLineWidth(2);
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    list<Point2DWithAlpha> values = data();
    Point2DWithAlpha last;
    BOOST_FOREACH(Point2DWithAlpha point, values){
        if(!first){
            glColor4f(0.,1.0,0.,last.alpha);
            glVertex2f(last.x,last.y);
            glColor4f(0.,1.0,0.,point.alpha);
            glVertex2f(point.x,point.y);
        }
        last = point;
        first = false;
    }
    glEnd();
}

void GLPlotWidget2d::clear_lines(){
    m_x_lines.clear();
    m_y_lines.clear();
}
