#include "drawable.h"
#include "drawableneuron.h"
#include "drawableaxon.h"
#include "drawableaxonnode.h"
#include "drawabledendritenode.h"

void Drawable::init_with_object(SimulationObject* object){
    m_object = object;
}

std::set<Drawable*> Drawables::m_all_drawables;

Drawables& Drawables::instance(){
    static Drawables dr;
    return dr;
}

std::set<Drawable*> Drawables::get_all_drawables(){
    return m_all_drawables;
}

Drawables::Drawables(){
    m_all_drawables.insert(new DrawableNeuron);
    m_all_drawables.insert(new DrawableAxon);
    m_all_drawables.insert(new DrawableAxonNode);
    m_all_drawables.insert(new DrawableDendriteNode);
}
