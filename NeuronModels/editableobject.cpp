#include "editableobject.h"
#include <boost/foreach.hpp>

std::map<std::string, boost::any> EditableObject::properties(){
    return std::map<std::string, boost::any>();
}

void EditableObject::set_property(std::string, boost::any){

}

void EditableObject::set_properties(std::map<std::string, boost::any> properties){
    std::pair<std::string,boost::any> p;
    BOOST_FOREACH(p, properties){
        set_property(p.first, p.second);
    }
}
