#include "group.h"
#include "simulation.h"
#include <boost/foreach.hpp>

Group::Group(Simulation* sim)
    : SimulationObject(sim), m_update_time_interval(0.), m_elapsed_sim_time(0.), m_drawn_horizontally(true)
{
}



void Group::update(double milli_seconds){
    m_elapsed_sim_time += milli_seconds;
    if(m_elapsed_sim_time < m_update_time_interval) return;

    m_elapsed_sim_time -= m_update_time_interval;
}

std::list<std::string> Group::user_actions(){
    return std::list<std::string>();
}

void Group::do_user_action(std::string){

}

std::set<SimulationObject*> Group::about_to_remove(SimulationObject* object){
    std::set<SimulationObject*> also_to_remove;
    if(this == object){
        BOOST_FOREACH(SimulationObject* o, m_objects){
            also_to_remove.insert(o);
        }
    }
    return also_to_remove;
}

void Group::set_user_action_script(QString script){
    m_user_actions_script = script;
}

QString Group::user_action_script(){
    return m_user_actions_script;
}

void Group::set_update_script(QString script){
    m_update_script = script;
}

QString Group::update_script(){
    return m_update_script;
}

void Group::set_update_time_interval(double d){
    m_update_time_interval = d;
}

double Group::update_time_interval(){
    return m_update_time_interval;
}

void Group::set_drawn_horizontally(bool b){
    m_drawn_horizontally = b;
}

bool Group::drawn_horizontally() const{
    return m_drawn_horizontally;
}

void Group::add_input(Samples* input){
    m_inputs.insert(input);
}

void Group::remove_input(Samples* input){
    m_inputs.erase(input);
}

void Group::add_object(SimulationObject* object){
    m_objects.insert(object);
}

void Group::remove_object(SimulationObject* object){
    m_objects.erase(object);
}

std::set<SimulationObject*> Group::objects_as_std_set() const{
    return m_objects;
}

QList<SimulationObject*> Group::objects() const{
    QList<SimulationObject*> return_list;
    BOOST_FOREACH(SimulationObject* o, objects_as_std_set()){
        return_list.append(o);
    }
    return return_list;
}

Point Group::handle_position() const{
    double min_z,max_z,min_y,max_y,min_x,max_x;
    min_z=max_z=min_y=max_y=min_x=max_x=0;
    Point pos;
    Point center = position();
    BOOST_FOREACH(SimulationObject* o, objects_as_std_set()){
        Point n_pos = o->position();
        pos = n_pos - center;
        if(pos.z < min_z) min_z = pos.z;
        if(pos.z > max_z) max_z = pos.z;
        if(pos.y < min_y) min_y = pos.y;
        if(pos.y > max_y) max_y = pos.y;
        if(pos.x < min_x) min_x = pos.x;
        if(pos.x > max_x) max_x = pos.x;
    }

    min_z -= MARGIN;
    min_y -= MARGIN;
    min_x -= MARGIN;
    max_z += MARGIN;
    max_y += MARGIN;
    max_x += MARGIN;

    if(drawn_horizontally())
        return Point(min_x,0,max_z);
    else
        return Point(0,min_y,max_z);
}

Point Group::moving_offset() const{
    return handle_position();
}

void Group::moved(Point new_position, Point old_position){
    Point vec = new_position - old_position;
    foreach(SimulationObject* o, objects()){
        o->set_position(o->position() + vec);
    }
}
