#include "neuron_membrane_plot_widget.h"
#include "ui_neuron_membrane_plot_widget.h"
#include <boost/foreach.hpp>
#include "neuron.h"
#include "application.h"
#include "simulation.h"

typedef std::pair<double,double> double_pair;

NeuronMembranePlotWidget::NeuronMembranePlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeuronMembranePlotWidget),
    m_plot(this)
{
    ui->setupUi(this);
    QLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(&m_plot);
    ui->frame->setLayout(layout);
    connect(ui->last_milliseconds, SIGNAL(valueChanged(double)), &m_plot, SLOT(set_time_intervall(double)));
    m_plot.set_time_intervall(ui->last_milliseconds->value());
}

NeuronMembranePlotWidget::~NeuronMembranePlotWidget()
{
    delete ui;
}

void NeuronMembranePlotWidget::set_neuron(Neuron* n){
    m_plot.set_neuron(n);
}

GLMembranePlot::GLMembranePlot(QWidget* parent) :
        GLPlotWidget2d(parent),
        m_neuron(0),
        m_time(0)
{
    setup();
    connect(&Application::instance(), SIGNAL(refresh()), this, SLOT(updateGL()));
    connect(Application::instance().simulation(), SIGNAL(simulation_started()), this, SLOT(simulation_started()));
}

void GLMembranePlot::setup(){
    set_region(-m_time_interval,-105,0,35);
    set_x_unit("ms");
    set_y_unit("mV");
    //set_number_of_x_lines(6);
    //set_number_of_y_lines(5);
    clear_lines();
    add_y_line_at(0,false);
    add_y_line_at(-35,true);
    add_y_line_at(-70,false);
    add_x_line_at(-m_time_interval/4,true);
    add_x_line_at(-m_time_interval/2,true);
    add_x_line_at(-m_time_interval/4*3,true);

}

void GLMembranePlot::set_neuron(Neuron* n){
    if(m_neuron != n) m_last_values.clear();
    m_neuron = n;

}

void GLMembranePlot::set_time_intervall(double milliseconds){
    m_time_interval = milliseconds;
    setup();
}

void GLMembranePlot::update_values(){
    if(!m_neuron)
        return;

    double new_time = Application::instance().simulation()->time_ms();
    if(m_time == new_time) return;
    double milliseconds = new_time - m_time;
    m_time = new_time;
    BOOST_FOREACH(double_pair& p, m_last_values){
        p.first -= milliseconds;
    }

    while((!m_last_values.empty()) && (m_last_values.back().first > m_time_interval)) m_last_values.pop_back();

    m_last_values.push_front(double_pair(0,m_neuron->membrane_potential()));
}

list<GLPlotWidget2d::Point2DWithAlpha> GLMembranePlot::data(){
    list<GLPlotWidget2d::Point2DWithAlpha> return_value;

    update_values();
    BOOST_FOREACH(double_pair p, m_last_values){
        GLPlotWidget2d::Point2DWithAlpha point;
        point.x = p.first;
        point.y = p.second;
        point.alpha = (m_time_interval + point.x)/m_time_interval;
        return_value.push_back(point);
    }

    return return_value;
}

void GLMembranePlot::simulation_started(){
    m_time = 0;
}
