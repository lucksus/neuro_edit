#include "drawable.h"
#include "drawableneuron.h"
#include "drawableaxon.h"
#include "drawableaxonnode.h"
#include "drawabledendritenode.h"
#include "drawabledendriteconnection.h"
#include "drawablesynapse.h"
#include <assert.h>

void Drawable::init_with_object(SimulationObject* object){
    m_object = object;
}

void Drawable::draw_geometry(){
    if(m_display_list != 0){
        glCallList(m_display_list);
    }else{
        m_display_list=glGenLists(1);
        assert(m_display_list != 0);
        glNewList(m_display_list,GL_COMPILE);
        draw_geometry_impl();
        glEndList();
    }
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
    m_all_drawables.insert(new DrawableDendriteConnection);
    m_all_drawables.insert(new DrawableSynapse);
}
