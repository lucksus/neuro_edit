#ifndef EDITABLEOBJECT_H
#define EDITABLEOBJECT_H
#include <boost/any.hpp>
#include <map>
#include <string>

class EditableObject
{
public:
    virtual std::map<std::string, boost::any> properties();
    virtual void set_property(std::string, boost::any);
    virtual void set_properties(std::map<std::string, boost::any>);
};

#endif // EDITABLEOBJECT_H
