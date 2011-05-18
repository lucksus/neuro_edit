#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <set>
#include <OpenGL/gl.h>

class SimulationObject;
class Drawable
{
public:
    Drawable():m_display_list(0){}
    virtual bool is_applicable_to(SimulationObject*) = 0;
    virtual void init_with_object(SimulationObject*);
    virtual void set_color_and_lightning() = 0;
    void draw_geometry();
    virtual void draw_geometry_impl() = 0;

protected:
    SimulationObject* m_object;
    GLuint m_display_list;
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
