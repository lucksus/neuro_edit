#include "neuron_membrane_plot_widget.h"
#include "ui_neuron_membrane_plot_widget.h"
#include <boost/foreach.hpp>
#include "neuron.h"

typedef std::pair<double,double> double_pair;

NeuronMembranePlotWidget::NeuronMembranePlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeuronMembranePlotWidget),
    m_plot(this)
{
    ui->setupUi(this);
    QLayout* layout = new QHBoxLayout();
    layout->addWidget(&m_plot);
    ui->frame->setLayout(layout);
    connect(ui->last_milliseconds, SIGNAL(valueChanged(double)), &m_plot, SLOT(set_time_intervall(double)));
    m_plot.set_time_intervall(ui->last_milliseconds->value());
}

NeuronMembranePlotWidget::~NeuronMembranePlotWidget()
{
    delete ui;
}

void NeuronMembranePlotWidget::milliseconds_passed(double milliseconds){
    m_plot.milliseconds_passed(milliseconds);
}

void NeuronMembranePlotWidget::set_neuron(Neuron* n){
    m_plot.set_neuron(n);
}

GLMembranePlot::GLMembranePlot(QWidget* parent) :
        QGLWidget(parent),
        m_neuron(0)
{

}

void GLMembranePlot::set_neuron(Neuron* n){
    m_neuron = n;
    if(!m_neuron) m_last_values.clear();
    updateGL();
}

void GLMembranePlot::set_time_intervall(double milliseconds){
    m_time_interval = milliseconds;
    updateGL();
}

void GLMembranePlot::milliseconds_passed(double milliseconds){
    if(!m_neuron)
        return;

    BOOST_FOREACH(double_pair& p, m_last_values){
        p.first -= milliseconds;
    }

    while((!m_last_values.empty()) && (m_last_values.back().first > m_time_interval)) m_last_values.pop_back();

    m_last_values.push_front(double_pair(0,m_neuron->membrane_potential()));
    updateGL();
}


void GLMembranePlot::initializeGL(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLMembranePlot::resizeGL(int w, int h){
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void GLMembranePlot::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setup_projection_matrix();
    paint_axis();
    paint_values();
    glFlush();
}


void GLMembranePlot::setup_projection_matrix(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-110, -110, 120, 120);
    gluOrtho2D(-100, -100, 120, 120);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glScalef(0.018,0.019,1.);
    //glTranslatef(65,55,0);

    glScalef(1/50.,1/70.,1.);
    glTranslatef(50,35,0);

}

void GLMembranePlot::paint_axis(){
    glColor3f(.7,.7,.7);
    glLineStipple (1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);

    glLineWidth(1);
    glBegin(GL_LINES);
    for(int i = -105; i<=35; i+=35){
        glVertex2f(-100,i); glVertex2f(0,i);
    }

    for(int i = -100; i<=0; i+=20){
        glVertex2f(i,-105); glVertex2f(i,35);
    }

    //glVertex2f(-100,0);glVertex2f(0,0);
    //glVertex2f(-100,-0.5);glVertex2f(0,-0.5);
    glEnd();

    glDisable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2f(-100,0);glVertex2f(0,0);
    glVertex2f(-100,-70);glVertex2f(0,-70);
    glEnd();

    QFont font;
    QFontMetrics metrics(font);
    QString text = QString("0");
    QRect boundingRect = metrics.boundingRect(text);
    int h = boundingRect.height()/2;

    glColor3f(1.,1.,1.);
    renderText(0.,height()/4 + h,QString("0mV"));
    //renderText(0.,height()/2 + h,QString("-35"));
    renderText(0.,height()*3/4 + h,QString("-70mV"));

    text = QString("%1ms").arg(-m_time_interval*4/5);
    renderText(width()/5 - metrics.boundingRect(text).width()/2, height(), text);
    text = QString("%1ms").arg(-m_time_interval*3/5);
    renderText(width()*2/5 - metrics.boundingRect(text).width()/2, height(), text);
    text = QString("%1ms").arg(-m_time_interval*4/5);
    renderText(width()*3/5 - metrics.boundingRect(text).width()/2, height(), text);
    text = QString("%1ms").arg(-m_time_interval*2/5);
    renderText(width()*4/5 - metrics.boundingRect(text).width()/2, height(), text);

}

void GLMembranePlot::paint_values(){
    double_pair last;
    bool first = true;
    glLineWidth(2);
    glBegin(GL_LINES);
    BOOST_FOREACH(double_pair p, m_last_values){
        if(!first){
            double a = last.first/m_time_interval;
            double b = p.first/m_time_interval;
            glColor4f(0.,1.0,0.,1. + a);
            glVertex2f(a*100,last.second);
            glColor4f(0.,1.0,0.,1. + b);
            glVertex2f(b*100,p.second);
        }
        last = p;
        first = false;
    }
    glEnd();
}
