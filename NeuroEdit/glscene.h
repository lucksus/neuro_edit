#ifndef GLSCENE_H
#define GLSCENE_H
#include <QGLWidget>
#include <set>
#include "network.h"
#include "neuron.h"
#include <QTimer>
#include <QTime>

class CurrentInducer;
class Drawable;
class GLScene : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLScene(QWidget *parent = 0);
    ~GLScene();

    std::set<SimulationObject*> selected_objects();

    enum SimulationObjectHacks{DRAWABLE_POINTER=1, PICKING_DISPLAY_LIST=2};


    template<class SimObjectType>
    std::pair<SimulationObject*,double> find_nearest_2d(const int& mousex, const int& mousey){
        makeCurrent();
        setup_projection_and_modelview_matrix();
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT,viewport);
        GLdouble model_view[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
        GLdouble projection[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

        SimulationObject* nearest = 0;
        double distance;
        BOOST_FOREACH(SimulationObject* o, m_network->objects_with_children_as_std_set()){
            if(dynamic_cast<SimObjectType>(o)){
                GLdouble x,y,z;
                gluProject(o->position().x, o->position().y, o->position().z, model_view, projection, viewport, &x, &y, &z);
                y = height() - y;
                Point a(x,y,0);
                Point b(mousex,mousey,0);
                double dist = a.distance(b);
                if(nearest == 0){
                    nearest = o;
                    distance = dist;
                }else if(dist < distance){
                    nearest = o;
                    distance = dist;
                }
            }
        }
        return std::pair<SimulationObject*,double>(nearest,distance);
    }

signals:
    void selection_changed(std::set<SimulationObject*>);
    void neuron_selected(Neuron*);
    void user_interaction();

public slots:
    void set_network(Network*);
    void start_inserting(std::set<SimulationObject*>);
    void deselect();
    void start_connecting(SimulationObject*);
    void start_inserting_current_inducer();

protected:
     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();

     void mouseMoveEvent(QMouseEvent *e);
     void mousePressEvent(QMouseEvent *e);
     void mouseReleaseEvent(QMouseEvent *e);
     void mouseDoubleClickEvent(QMouseEvent *e);
     void wheelEvent(QWheelEvent *e);
     void keyPressEvent(QKeyEvent *e);
     void keyReleaseEvent(QKeyEvent *e);

private slots:
     void object_deleted(SimulationObject*);

private:
    Network* m_network;
    std::set<SimulationObject*> m_selected_objects;
    void add_to_selection(SimulationObject*);
    void select(SimulationObject*);
    void select(std::set<SimulationObject*>);

    void setup_projection_and_modelview_matrix();
    void apply_camera_translation_to_modelviewmatrix();

    struct CameraConfig{
        Point center_position;
        double distance, azimuth, elevation;
        Point position();
    } m_camera_config;

    void paint_floor();
    void paint_objects(bool picking_run = false);
    void paint_object(SimulationObject*, bool picking=false, bool moving=false);
    void paint_selection_marker(QRect);


    bool m_mousedown_right, m_mousedown_left;
    int m_oldMouseX;
    int m_oldMouseY;
    bool m_shift_key_down, m_ctrl_key_down;

    //----------------
    // selection box
    //----------------
    int m_selection_box_origin[2];
    int m_selection_box_current[2];
    bool m_selection_box;
    void paint_selection_box();
    std::set<SimulationObject*> objects_in_selection_box();


    GLubyte* m_pixel_buffer;
    SimulationObject* object_under_cursor(int cursorX, int cursorY);

    Point mouse_on_plane(int x, int y, Point plane_origin, Point normal);

    QRect occupied_2d_region_of_object(SimulationObject*);


    //-----------------
    //moving:
    //-----------------
    bool m_moving, m_insert_moving;
    Point m_moving_start_point;
    Point m_moving_point;
    Point m_moving_switch_plane_point;
    std::set<SimulationObject*> m_moving_objects;
    void start_moving(const SimulationObject&);
    void finish_moving();
    void abort_moving();
    void paint_moving_plane();




    //-----------------
    //camera moving:
    //-----------------
    Point m_camera_center_moving_target, m_camera_center_moving_source;
    double m_camera_center_moving_param;
    QTimer m_camera_center_moving_timer;


    //-----------------
    //connecting:
    //-----------------
    bool m_connecting;
    SimulationObject* m_connection_source;
    SimulationObject* m_current_connection_target;
    void finish_connecting();
    void paint_connecting_overlay();


    //-----------------
    //inserting current inducer:
    //-----------------
    bool m_inserting_current_inducer;
    CurrentInducer* m_phony_current_inducer;
    void finish_inserting_current_inducer(SimulationObject*);


private slots:
    void camera_center_moving_update();
private:

    QTime m_last_paint_time;
    bool m_currently_painting;
    double m_fov;
};

#endif // GLSCENE_H
