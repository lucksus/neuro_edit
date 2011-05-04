#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H
#include "qttreepropertybrowser.h"
#include <set>
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
    std::set<QObject*> m_editable_objects;
    QtVariantPropertyManager *m_variantManager;
    std::map<QtVariantProperty*, std::string> m_property_to_name;
    std::map<QtVariantProperty*, QtProperty*> m_property_groups;
    bool m_populating;

    void populate_properties(std::set<std::string> properties_to_show);
    static std::set<std::string> properties(QObject* object);
    static std::set<std::string> intersect(const std::set<std::string>& a, const std::set<std::string>& b);

};

#endif // PROPERTYBROWSER_H
