#include "propertybrowser.h"
#include "editableobject.h"
#include <boost/foreach.hpp>
#include "simulationobject.h"
#include <iostream>

PropertyBrowser::PropertyBrowser(QWidget* parent)
    : QtTreePropertyBrowser(parent), m_populating(false)
{
     m_variantManager = new QtVariantPropertyManager();
     QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();

     setFactoryForManager(m_variantManager, variantFactory);
     setPropertiesWithoutValueMarked(true);
     setRootIsDecorated(false);

     connect(m_variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(value_changed(QtProperty*,QVariant)));
}

void PropertyBrowser::objects_selected(std::set<SimulationObject *> objects){

    m_editable_objects.clear();
    m_property_to_group_and_name.clear();
    BOOST_FOREACH(SimulationObject* o, objects){
        EditableObject* e = dynamic_cast<EditableObject*>(o);
        if(e) m_editable_objects.insert(e);
    }

    bool first = true;
    Properties properties;
    BOOST_FOREACH(EditableObject* e, m_editable_objects){
        if(first){
            properties = e->properties();
            first = false;
        }else{
            properties.intersect(e->properties());
        }
    }

    populate_properties(properties);

}

void PropertyBrowser::read_values_from_objects(){
    if(m_populating || m_editable_objects.empty()) return;
    EditableObject* object = *m_editable_objects.begin();
    Properties properties = object->properties();
    std::pair<QtVariantProperty*, std::pair<std::string, std::string> > it;
    BOOST_FOREACH(it, m_property_to_group_and_name){
            it.first->setValue(any_to_variant(properties.value(it.second.first, it.second.second)));
    }
}

void PropertyBrowser::value_changed(QtProperty * p, const QVariant & value){
    if(m_populating || m_editable_objects.empty()) return;
    QtVariantProperty* property = dynamic_cast<QtVariantProperty*>(p);
    if(!property) return;
    EditableObject* object = *m_editable_objects.begin();
    Properties properties = object->properties();
    std::string group = m_property_to_group_and_name.find(property)->second.first;
    std::string name = m_property_to_group_and_name.find(property)->second.second;
    boost::any any_value = variant_to_any(value, properties.value(group,name));

    BOOST_FOREACH(EditableObject* object, m_editable_objects){
        object->set_property(group, name, any_value);
    }
}


void PropertyBrowser::populate_properties(Properties properties_to_show){
    m_populating = true;
    clear();

    if(m_editable_objects.empty()) return;
    EditableObject* object = *m_editable_objects.begin();
    Properties properties = object->properties();

    BOOST_FOREACH(std::string group, properties_to_show.groups()){
        QtProperty* group_item = 0;
        if(group.length()) group_item = m_variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), group.c_str());
        BOOST_FOREACH(std::string name, properties_to_show.properties(group)){
            boost::any value = properties.value(group, name);
            QtVariantProperty* property = any_to_property(value, name);
            if(!property) continue;
            if(group_item) group_item->addSubProperty(property);
            else addProperty(property);
            m_property_to_group_and_name[property] = std::pair<std::string,std::string>(group,name);
        }
        if(group_item) addProperty(group_item);
    }
    m_populating = false;
}

QVariant PropertyBrowser::any_to_variant(boost::any value){
    QVariant variant;
    try{
        double d = boost::any_cast<double>(value);
        variant = d;
    }catch(boost::bad_any_cast){
    }
    return variant;
}

QtVariantProperty* PropertyBrowser::any_to_property(boost::any value, std::string name){
    QtVariantProperty* property = 0;
    try{
        double d = boost::any_cast<double>(value);
        property = m_variantManager->addProperty(QVariant::Double, name.c_str());
        property->setValue(d);
    }catch(boost::bad_any_cast){
    }
    return property;
}

boost::any PropertyBrowser::variant_to_any(const QVariant& value, boost::any any_reference){
    boost::any result;
    try{
        boost::any_cast<double>(any_reference);
        result = value.toDouble();

    }catch(boost::bad_any_cast){
    }

    return result;
}
