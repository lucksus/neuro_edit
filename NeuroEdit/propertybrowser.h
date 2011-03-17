#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H
#include "qttreepropertybrowser.h"
#include <set>
#include "editableobject.h"
#include "qtvariantproperty.h"

class SimulationObject;
class QtVariantPropertyManager;
class PropertyBrowser : public QtTreePropertyBrowser
{
Q_OBJECT
public:
    PropertyBrowser(QWidget* parent);

public slots:
    void objects_selected(std::set<SimulationObject*>);
    void read_values_from_objects();

private slots:
    void value_changed(QtProperty * property, const QVariant & value);

private:
    std::set<EditableObject*> m_editable_objects;
    QtVariantPropertyManager *m_variantManager;
    std::map<QtVariantProperty*, std::pair<std::string,std::string> > m_property_to_group_and_name;
    bool m_populating;

    void populate_properties(Properties properties_to_show);
    QVariant any_to_variant(boost::any value);
    QtVariantProperty* any_to_property(boost::any value, std::string name);
    boost::any variant_to_any(const QVariant& value, boost::any any_reference);

};

#endif // PROPERTYBROWSER_H
