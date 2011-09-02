#include "drawable.h"
#include "drawableneuron.h"
#include "drawableaxon.h"
#include "drawableaxonnode.h"
#include "drawabledendritenode.h"
#include "drawablesynapse.h"
#include "drawablecurrentinducer.h"
#include "drawablesamples.h"
#include "drawablegroup.h"
#include "drawablelineardiscriminator.h"
#include "drawableldconnection.h"
#include <assert.h>
#include "simulationobject.h"
#include "glscene.h"
#include <boost/tuple/tuple_comparison.hpp>

boost::tuple<GLuint,GLuint,GLuint> Drawable::s_next_picking_name;
std::map<boost::tuple<GLuint,GLuint,GLuint>, SimulationObject*> Drawable::s_picking_names;

void Drawable::draw(){
    if(m_dont_use_display_lists){
        set_color_and_lightning();
        draw_geometry_impl();
        return;
    }

    if(m_display_list != 0 && !m_dont_use_display_lists){
        glCallList(m_display_list);
    }else{
        m_display_list=glGenLists(1);
        assert(m_display_list != 0);
        glNewList(m_display_list,GL_COMPILE_AND_EXECUTE);
        set_color_and_lightning();
        draw_geometry_impl();
        glEndList();
    }
}

void Drawable::draw_moving(){
    if(m_moving_display_list != 0 && !m_dont_use_display_lists){
        glCallList(m_moving_display_list);
    }else{
        m_moving_display_list=glGenLists(1);
        assert(m_moving_display_list != 0);
        glNewList(m_moving_display_list,GL_COMPILE_AND_EXECUTE);
        glEnable(GL_LIGHTING);
        glEnable(GL_DITHER);
        glDisable(GL_CULL_FACE);
        GLfloat transparent[] = {.9,.9,.9,0.5};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transparent);
        draw_geometry_impl();
        glEndList();
    }
}


void Drawable::draw_picking(){
    void* &list = m_object->bad_hacks[GLScene::PICKING_DISPLAY_LIST];
    if(list != 0 && !m_dont_use_display_lists){
        glCallList(*static_cast<GLuint*>(list));
    }else{
        if(!m_dont_use_display_lists){
            if(list == 0) list = new GLuint;
            GLuint* real_list = static_cast<GLuint*>(list);
            *real_list = glGenLists(1);
            assert(*real_list != 0);
            glNewList(*real_list,GL_COMPILE_AND_EXECUTE);
        }
        increment_picking_name();
        glColor3ub(s_next_picking_name.get<0>(),s_next_picking_name.get<1>(),s_next_picking_name.get<2>());
        s_picking_names[s_next_picking_name] = m_object;
        glDisable(GL_DITHER);
        glDisable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);
        draw_geometry_impl();
        if(!m_dont_use_display_lists)
            glEndList();
    }
}

void Drawable::increment_picking_name(){
    GLuint a,b,c;
    a = s_next_picking_name.get<0>();
    b = s_next_picking_name.get<1>();
    c = s_next_picking_name.get<2>();
    a++;
    if(a>255){
        a = 0; b++;
    }
    if(b>255){
        b = 0; c++;
    }
    assert(c<=255);
    s_next_picking_name = boost::make_tuple(a,b,c);
}

SimulationObject* Drawable::object_for_picking_name(const boost::tuple<GLuint,GLuint,GLuint>& name){
    std::map<boost::tuple<GLuint,GLuint,GLuint>, SimulationObject*>::iterator it = s_picking_names.find(name);
    if(it == s_picking_names.end()) return 0;
    return it->second;
}

void Drawable::reset_display_lists(){
    m_display_list = 0;
    m_moving_display_list = 0;
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
    m_all_drawables.insert(new DrawableSynapse);
    m_all_drawables.insert(new DrawableCurrentInducer);
    m_all_drawables.insert(new DrawableSamples);
    m_all_drawables.insert(new DrawableGroup);
    m_all_drawables.insert(new DrawableLinearDiscriminator);
    m_all_drawables.insert(new DrawableLDConnection);
}
