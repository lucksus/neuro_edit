#include "propertybrowser.h"
#include "editableobject.h"
#include <boost/foreach.hpp>
#include "simulationobject.h"
#include <iostream>
#include <QMetaObject>
#include <QMetaProperty>
#include "point.h"

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
    m_property_to_name.clear();
    BOOST_FOREACH(SimulationObject* s, objects){
        m_editable_objects.insert(s);
    }

    bool first = true;
    std::set<std::string> props;
    BOOST_FOREACH(QObject* o, m_editable_objects){
        if(first){
            props = properties(o);
            first = false;
        }else{
            props = intersect(properties(o), props);
        }
    }

    populate_properties(props);

}

void PropertyBrowser::read_values_from_objects(){
    if(m_populating || m_editable_objects.empty()) return;
    std::set<SimulationObject*> editable_objects;
    BOOST_FOREACH(QObject* obj, m_editable_objects){
        editable_objects.insert(static_cast<SimulationObject*>(obj));
    }
    objects_selected(editable_objects);
}

void PropertyBrowser::value_changed(QtProperty * p, const QVariant & value){
    if(m_populating || m_editable_objects.empty()) return;
    QtVariantProperty* property = dynamic_cast<QtVariantProperty*>(p);
    if(!property) return;

    BOOST_FOREACH(QObject* object, m_editable_objects){
        if(m_property_groups.count(property)){
            QtProperty* group = m_property_groups.find(property)->second;
            if(group->propertyName() == "position"){
                Point p = object->property("position").value<Point>();
                if(property->propertyName() == "x") p.x = value.value<double>();
                if(property->propertyName() == "y") p.y = value.value<double>();
                if(property->propertyName() == "z") p.z = value.value<double>();
                QVariant v;
                v.setValue(p);
                object->setProperty("position", v);
            }
        }
        object->setProperty(property->propertyName().toStdString().c_str(), value);
    }
}


void PropertyBrowser::populate_properties(std::set<std::string> properties_to_show){
    m_populating = true;
    clear();

    if(m_editable_objects.empty()) return;
    QObject* object = *m_editable_objects.begin();

    BOOST_FOREACH(std::string property, properties_to_show){

        QVariant property_value = object->property(property.c_str());
        QVariant::Type type = property_value.type();

        if(QVariant::UserType == type){
            if(QMetaType::type("Point") == QMetaType::type(property_value.typeName())){
                Point p = property_value.value<Point>();
                QtProperty* group_item = m_variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), property.c_str());
                QtVariantProperty* x = m_variantManager->addProperty(QVariant::Double, "x");
                QtVariantProperty* y = m_variantManager->addProperty(QVariant::Double, "y");
                QtVariantProperty* z = m_variantManager->addProperty(QVariant::Double, "z");
                x->setValue(p.x);
                y->setValue(p.y);
                z->setValue(p.z);
                group_item->addSubProperty(x);
                group_item->addSubProperty(y);
                group_item->addSubProperty(z);
                m_property_groups[x] = m_property_groups[y] = m_property_groups[z] = group_item;
                addProperty(group_item);
            }
        }else{
            QtVariantProperty* variant_property = m_variantManager->addProperty(type, property.c_str());
            variant_property->setValue(property_value);
            addProperty(variant_property);
        }
    }
    m_populating = false;
}

std::set<std::string> PropertyBrowser::properties(QObject* object){
    std::set<std::string> props;
    for(int i=0; i < object->metaObject()->propertyCount(); i++){
        props.insert(object->metaObject()->property(i).name());
    }
    return props;
}

std::set<std::string> PropertyBrowser::intersect(const std::set<std::string>& a, const std::set<std::string>& b){
    std::set<std::string> result;
    BOOST_FOREACH(std::string s, a){
        if(b.count(s) == 0) continue;
        result.insert(s);
    }
    return result;
}
