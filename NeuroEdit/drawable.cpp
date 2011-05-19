#include "drawable.h"
#include "drawableneuron.h"
#include "drawableaxon.h"
#include "drawableaxonnode.h"
#include "drawabledendritenode.h"
#include "drawabledendriteconnection.h"
#include "drawablesynapse.h"
#include <assert.h>
#include "simulationobject.h"
#include "glscene.h"

boost::tuple<GLuint,GLuint,GLuint> Drawable::s_next_picking_name;
std::list< std::pair<boost::tuple<GLuint,GLuint,GLuint>, SimulationObject*> >Drawable::s_picking_names;

void Drawable::draw(){
    if(m_display_list != 0){
        glCallList(m_display_list);
    }else{
        m_display_list=glGenLists(1);
        assert(m_display_list != 0);
        glNewList(m_display_list,GL_COMPILE);
        set_color_and_lightning();
        draw_geometry_impl();
        glEndList();
    }
}

void Drawable::draw_moving(){
    if(m_moving_display_list != 0){
        glCallList(m_moving_display_list);
    }else{
        m_moving_display_list=glGenLists(1);
        assert(m_moving_display_list != 0);
        glNewList(m_moving_display_list,GL_COMPILE);
        glEnable(GL_LIGHTING);
        glEnable(GL_DITHER);
        GLfloat transparent[] = {.9,.9,.9,0.5};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transparent);
        draw_geometry_impl();
        glEndList();
    }
}

bool operator<(const boost::tuple<GLuint,GLuint,GLuint>& a, const boost::tuple<GLuint,GLuint,GLuint>& b){
    return a.get<0>() < b.get<0>() && a.get<1>() < b.get<1>() && a.get<2>() < b.get<2>();
}

void Drawable::draw_picking(){
    void* &list = m_object->bad_hacks[GLScene::PICKING_DISPLAY_LIST];
    if(list != 0){
        glCallList(*static_cast<GLuint*>(list));
    }else{
        list = new GLuint;
        GLuint* real_list = static_cast<GLuint*>(list);
        *real_list = glGenLists(1);
        assert(*real_list != 0);
        glNewList(*real_list,GL_COMPILE);
        increment_picking_name();
        GLuint r,g,b;
        r = s_next_picking_name.get<0>();
        g = s_next_picking_name.get<1>();
        b = s_next_picking_name.get<2>();
        glColor3ub(r,g,b);
        s_picking_names.push_back( std::pair<boost::tuple<GLuint,GLuint,GLuint>, SimulationObject*>(s_next_picking_name,m_object) );
        glDisable(GL_DITHER);
        glDisable(GL_LIGHTING);
        draw_geometry_impl();
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
    std::pair<boost::tuple<GLuint,GLuint,GLuint>, SimulationObject*> it;
    BOOST_FOREACH(it, s_picking_names){
        boost::tuple<GLuint,GLuint,GLuint> bla = it.first;
        if(bla.get<0>() == name.get<0>() && bla.get<1>() == name.get<1>() && bla.get<2>() == name.get<2>()) return it.second;
    }
    //assert(false);
    return 0;
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
