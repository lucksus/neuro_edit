#include "neuron_membrane_plot_widget.h"
#include "ui_neuron_membrane_plot_widget.h"

NeuronMembranePlotWidget::NeuronMembranePlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeuronMembranePlotWidget),
    m_plot(this)
{
    ui->setupUi(this);
    QLayout* layout = new QHBoxLayout();
    layout->addWidget(&m_plot);
    ui->frame->setLayout(layout);
}

NeuronMembranePlotWidget::~NeuronMembranePlotWidget()
{
    delete ui;
}

void NeuronMembranePlotWidget::on_last_milliseconds_changed(double value){

}

GLMembranePlot::GLMembranePlot(QWidget* parent) :
        QGLWidget(parent),
        m_neuron(0)
{

}

void GLMembranePlot::set_neuron(Neuron* n){
    m_neuron = n;
}

void GLMembranePlot::milliseconds_passed(double milliseconds){

}


void GLMembranePlot::initializeGL(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void GLMembranePlot::resizeGL(int w, int h){
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void GLMembranePlot::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setup_projection_matrix();
    paint_axis();
    glFlush();
}


void GLMembranePlot::setup_projection_matrix(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-110, -110, 120, 120);
    gluOrtho2D(-100, -100, 120, 120);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(0.018,0.019,1.);
    glTranslatef(65,55,0);

}

void GLMembranePlot::paint_axis(){
    glColor3f(.7,.7,.7);
    glBegin(GL_LINES);
    for(int i = -75; i<=0; i+=25){
        glVertex2f(-100,i); glVertex2f(0,i);
    }

    for(int i = -75; i<=0; i+=25){
        glVertex2f(i,-100); glVertex2f(i,0);
    }

    //glVertex2f(-1.5,-1.5); glVertex2f(-0.5,-0.5);
    glEnd();
    renderText(-112,-70,0,"-70");
}
