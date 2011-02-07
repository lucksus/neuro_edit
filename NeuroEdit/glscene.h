#ifndef GLSCENE_H
#define GLSCENE_H
#include <QGLWidget>
#include <set>
#include "network.h"
#include "neuron.h"

class GLScene : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLScene(QWidget *parent = 0);
    ~GLScene();

signals:
    void selection_changed(std::set<SimulationObject*>);
    void neuron_selected(Neuron*);

public slots:
    void set_network(Network*);

protected:
     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();

     void mouseMoveEvent(QMouseEvent *e);
     void mousePressEvent(QMouseEvent *e);
     void mouseReleaseEvent(QMouseEvent *e);
     void wheelEvent(QWheelEvent *e);
     void keyPressEvent(QKeyEvent *e);
     void keyReleaseEvent(QKeyEvent *e);



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
    void paint_objects(bool picking_run = false, bool only_moving_objects = false);


    bool m_mousedown_right, m_mousedown_left;
    int m_oldMouseX;
    int m_oldMouseY;
    bool m_shift_key_down, m_ctrl_key_down;

    GLuint* m_selection_buffer;
    GLuint m_selection_buffer_size;
    std::vector<SimulationObject*> m_picking_names;
    bool m_picking_run;

    int m_selection_box_origin[2];
    int m_selection_box_current[2];
    bool m_selection_box;
    void draw_selection_box();
    std::set<SimulationObject*> objects_in_selection_box();
    GLubyte* m_pixel_buffer;


    SimulationObject* object_under_cursor(int cursorX, int cursorY);
    Point mouse_on_plane(int x, int y, Point plane_origin, Point normal);

    //-----------------
    //moving:
    //-----------------
    bool m_moving;
    Point m_moving_start_point;
    Point m_moving_point;
    Point m_moving_switch_plane_point;
    std::set<SimulationObject*> m_moving_objects;
    void start_moving(const SpatialObject&);
    void finish_moving();
    void paint_moving_plane();

    double m_fov;
};

#endif // GLSCENE_H
