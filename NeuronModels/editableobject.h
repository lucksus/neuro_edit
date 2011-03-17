#ifndef EDITABLEOBJECT_H
#define EDITABLEOBJECT_H
#include <boost/any.hpp>
#include <map>
#include <string>
#include <set>

class Properties{
public:
    void add(std::string, boost::any);
    void set_group(std::string);

    std::set<std::string> groups() const;
    std::set<std::string> properties(std::string group) const;
    boost::any value(std::string group, std::string property) const;

    void merge(const Properties&);

private:
    std::string m_current_group;
    std::map<std::string, std::map<std::string, boost::any> > m_properties_in_groups;
};

class EditableObject
{
public:
    virtual Properties properties();
    virtual void set_property(std::string group, std::string name, boost::any value);
    virtual void set_properties(Properties);
};

#endif // EDITABLEOBJECT_H
