#include "propertybrowser.h"

PropertyBrowser::PropertyBrowser(QWidget* parent)
    : QtTreePropertyBrowser(parent)
{
}

void PropertyBrowser::objects_selected(std::set<SimulationObject *>){

}
