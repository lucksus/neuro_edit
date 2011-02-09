#include "izhikevich_systemstate_plotwidget.h"
#include <boost/foreach.hpp>
#include "izhikevich.h"

IzhikevichSystemStatePlotWidget::IzhikevichSystemStatePlotWidget(QWidget* parent):
        GLPlotWidget2d(parent),
        m_neuron(0)
{
    set_region(-20,-105,20,35);
    set_x_unit("");
    set_y_unit("mV");
    //set_number_of_x_lines(6);
    //set_number_of_y_lines(5);
    clear_lines();
    add_y_line_at(0,false);
    add_y_line_at(-35,true);
    add_y_line_at(-70,false);
    add_x_line_at(0,false);
    add_x_line_at(-35,true);
    add_x_line_at(-70,false);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_timer.start(1000/30);
}

void IzhikevichSystemStatePlotWidget::set_neuron(Neuron* n){
    Izhikevich* i = dynamic_cast<Izhikevich*>(n);
    m_neuron = i;
}

list<GLPlotWidget2d::Point2DWithAlpha> IzhikevichSystemStatePlotWidget::data(){
    BOOST_FOREACH(GLPlotWidget2d::Point2DWithAlpha& point, m_last_values){
        point.alpha -= 0.02;
    }

    while((!m_last_values.empty()) && (m_last_values.back().alpha <= 0)) m_last_values.pop_back();

    if(!m_neuron)
        return m_last_values;

    GLPlotWidget2d::Point2DWithAlpha point;
    point.alpha = 1;
    point.x = m_neuron->u();
    point.y = m_neuron->v();
    m_last_values.push_front(point);

    return m_last_values;
}
