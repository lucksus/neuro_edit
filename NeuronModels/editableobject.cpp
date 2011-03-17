#include "editableobject.h"
#include <boost/foreach.hpp>
//#include <pair>

void Properties::add(std::string name, boost::any value){
    m_properties_in_groups[m_current_group][name] = value;
}

void Properties::set_group(std::string group){
    m_current_group = group;
}

std::set<std::string> Properties::groups() const{
    std::set<std::string> result;
    std::pair<std::string, std::map<std::string,boost::any> > it;
    BOOST_FOREACH(it, m_properties_in_groups){
        result.insert(it.first);
    }
    return result;
}

std::set<std::string> Properties::properties(std::string group) const{
    std::set<std::string> result;
    std::pair<std::string,boost::any> it;
    std::map<std::string,boost::any> temp = m_properties_in_groups.find(group)->second;
    BOOST_FOREACH(it, temp){
        result.insert(it.first);
    }
    return result;
}

boost::any Properties::value(std::string group, std::string property) const{
    return m_properties_in_groups.find(group)->second.find(property)->second;
}

void Properties::merge(const Properties& p){
    BOOST_FOREACH(std::string group, p.groups()){
        BOOST_FOREACH(std::string name, p.properties(group))
                m_properties_in_groups[group][name] = p.value(group,name);
    }
}

void Properties::intersect(const Properties& p){
    std::map<std::string, std::map<std::string, boost::any> > temp;
    BOOST_FOREACH(std::string group, p.groups()){
        if(! groups().count(group)) continue;
        BOOST_FOREACH(std::string name, p.properties(group)){
                if(! properties(group).count(name)) continue;
                temp[group][name] = value(group,name);
        }
    }
    m_properties_in_groups = temp;
}





Properties EditableObject::properties(){
    return Properties();
}

void EditableObject::set_property(std::string, std::string, boost::any){

}

void EditableObject::set_properties(Properties properties){
    BOOST_FOREACH(std::string group, properties.groups()){
        BOOST_FOREACH(std::string name, properties.properties(group))
        set_property(group,name,properties.value(group,name));
    }
}
