#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H
#include "qttreepropertybrowser.h"
#include <set>

class SimulationObject;
class PropertyBrowser : public QtTreePropertyBrowser
{
Q_OBJECT
public:
    PropertyBrowser(QWidget* parent);

public slots:
    void objects_selected(std::set<SimulationObject*>);
};

#endif // PROPERTYBROWSER_H
