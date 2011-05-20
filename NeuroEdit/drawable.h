#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <set>
#include <OpenGL/gl.h>
#include <boost/tuple/tuple.hpp>
#include <map>

class SimulationObject;
class Drawable
{
public:
    Drawable():m_display_list(0),m_moving_display_list(0),m_dont_use_display_lists(false){}
    virtual bool is_applicable_to(SimulationObject*) = 0;
    inline void init_with_object(SimulationObject* object){m_object = object;}
    virtual void set_color_and_lightning() = 0;
    void draw();
    void draw_moving();
    void draw_picking();
    virtual void draw_geometry_impl() = 0;
    virtual bool do_translate_bevor_drawing() {return true;}

    static SimulationObject* object_for_picking_name(const boost::tuple<GLuint,GLuint,GLuint>&);

    void reset_display_lists();

protected:
    SimulationObject* m_object;
    GLuint m_display_list;
    GLuint m_moving_display_list;

    void dont_use_display_lists(){m_dont_use_display_lists=true;}

    static void increment_picking_name();
    static boost::tuple<GLuint,GLuint,GLuint> s_next_picking_name;
    static std::map<boost::tuple<GLuint,GLuint,GLuint>, SimulationObject*> s_picking_names;

private:
    bool m_dont_use_display_lists;
};




#include <set>

class Drawables{
public:
    static Drawables& instance();
    std::set<Drawable*> get_all_drawables();
private:
    Drawables();

    static std::set<Drawable*> m_all_drawables;
};

#endif // DRAWABLE_H
