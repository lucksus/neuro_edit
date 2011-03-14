#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <set>

class SimulationObject;
class Drawable
{
public:
    virtual bool is_applicable_to(SimulationObject*) = 0;
    virtual void init_with_object(SimulationObject*);
    virtual void set_color_and_lightning() = 0;
    virtual void draw_geometry() = 0;

protected:
    SimulationObject* m_object;
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
