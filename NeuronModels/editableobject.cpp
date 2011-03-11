#include "editableobject.h"

EditableObject::EditableObject()
{
}


std::map<std::string, boost::any> EditableObject::properties(){
    return std::map<std::string, boost::any>();
}

void EditableObject::set_property(std::string, boost::any){

}

void EditableObject::set_properties(std::map<std::string, boost::any>){
}
