#include "glscene.h"
#include <QMouseEvent>
#include <boost/foreach.hpp>
#include "neuron.h"
#include "link.h"
#include <GLUT/glut.h>
#include <algorithm>

GLScene::GLScene(QWidget *parent) :
    QGLWidget(parent), m_mousedown_right(false), m_mousedown_left(false), m_fov(120.),
    m_moving_start_point(0,0,0), m_picking_run(false), m_moving(false),
    m_shift_key_down(false), m_ctrl_key_down(false),
    m_selection_box(false)
{
    m_selection_buffer_size = 512;
    m_selection_buffer = new GLuint[m_selection_buffer_size];
    m_camera_config.distance = 100;
    setMouseTracking(true);
    setAutoBufferSwap(false);
    m_pixel_buffer = new GLubyte[2000*2000];
    m_camera_config.elevation = m_camera_config.azimuth = 45.;

    qRegisterMetaType< std::set<SimulationObject*> >("std::set<SimulationObject*>");
}

GLScene::~GLScene(){
    delete m_selection_buffer;
    delete[] m_pixel_buffer;
}

void GLScene::set_network(Network* network){
    m_network = network;
}

void GLScene::mouseMoveEvent(QMouseEvent *e){
    setFocus();
        //----------------
        //Camera:
        //----------------
        if(m_mousedown_right){
                int deltaX = e->x() - m_oldMouseX;
                int deltaY = -(e->y() - m_oldMouseY);
                m_camera_config.elevation -= deltaY;
                m_camera_config.azimuth += deltaX;
                if(m_camera_config.elevation >  90) m_camera_config.elevation =  90;
                if(m_camera_config.elevation < -90) m_camera_config.elevation = -90;
                updateGL();
        }


        if(m_mousedown_left){
            SimulationObject* clicked_object = 0;

            if(m_moving){
                Point normal(0,1,0);
                if(m_shift_key_down) normal = Point(0,0,1);
                if(m_ctrl_key_down) normal = Point(1,0,0);
                m_moving_point = mouse_on_plane(e->x(),e->y(),m_moving_switch_plane_point, normal);
            }else{
                if(m_selection_box){
                    m_selection_box_current[0] = e->x();
                    m_selection_box_current[1] = e->y();
                }else{
                    clicked_object = object_under_cursor(e->x(),e->y());
                    SpatialObject* spatial_object;
                    if(spatial_object = dynamic_cast<SpatialObject*>(clicked_object)){
                        //movable object dragged -> start moving:
                        if(!m_selected_objects.count(clicked_object))
                            m_selected_objects.clear();
                        if(m_selected_objects.empty())
                            add_to_selection(clicked_object);

                        start_moving(*spatial_object);
                    }else{
                        //background dragged -> start multiselection
                        m_selection_box_origin[0] = e->x();
                        m_selection_box_origin[1] = e->y();
                        m_selection_box_current[0] = e->x();
                        m_selection_box_current[1] = e->y();
                        m_selection_box = true;
                    }
                }
            }
        }

        m_oldMouseX = e->x();
        m_oldMouseY = e->y();
        updateGL();
}

void GLScene::mousePressEvent(QMouseEvent* e){
    setFocus();
    if(e->button() == Qt::RightButton) m_mousedown_right = true;
    if(e->button() == Qt::LeftButton){
        m_mousedown_left = true;
    }

    m_oldMouseX = e->x();
    m_oldMouseY = e->y();


}

void GLScene::mouseReleaseEvent(QMouseEvent* e){
    setFocus();
        m_mousedown_right = false;
        m_mousedown_left = false;
        if(e->button() == Qt::LeftButton){
            if(m_moving){
                finish_moving();
            }else{
                if(m_selection_box){
                    m_selection_box = false;
                    select(objects_in_selection_box());
                }else{

                    SimulationObject* o = object_under_cursor(e->x(),e->y());
                    if(m_shift_key_down)
                        add_to_selection(o);
                    else
                        select(o);
                }
                updateGL();
            }
        }
}

void GLScene::wheelEvent(QWheelEvent *e){
    setFocus();
        m_camera_config.distance -= e->delta()*pow(2,m_camera_config.distance/10000.f);
        if( m_camera_config.distance<70) m_camera_config.distance=70;
        if( m_camera_config.distance>10000) m_camera_config.distance=10000;
        updateGL();
}

void GLScene::keyPressEvent(QKeyEvent *e){
    int step = 4;
    switch(e->key()){
    case Qt::Key_A:
        m_camera_config.center_position.z -= step;
        break;
    case Qt::Key_S:
        m_camera_config.center_position.z += step;
        break;
    case Qt::Key_D:
        m_camera_config.center_position.x -= step;
        break;
    case Qt::Key_W:
        m_camera_config.center_position.x += step;
        break;
    case Qt::Key_Shift:
        m_shift_key_down = true;
        m_moving_switch_plane_point = m_moving_point;
        if(m_moving) updateGL();
        break;
    case Qt::Key_Control:
        m_ctrl_key_down = true;
        m_moving_switch_plane_point = m_moving_point;
        if(m_moving) updateGL();
        break;
    case Qt::Key_Escape:
        m_moving = false;
        updateGL();
        break;
    }

    updateGL();

    e->accept();
}

void GLScene::keyReleaseEvent(QKeyEvent *e){
    switch(e->key()){
    case Qt::Key_Shift:
        m_shift_key_down = false;
        m_moving_switch_plane_point = m_moving_point;
        if(m_moving) updateGL();
        break;
    case Qt::Key_Control:
        m_ctrl_key_down = false;
        m_moving_switch_plane_point = m_moving_point;
        if(m_moving) updateGL();
        break;
    }
}

void GLScene::start_moving(const SpatialObject& o){
    m_moving_objects = m_selected_objects;
    m_moving = true;
    m_moving_switch_plane_point = m_moving_start_point = m_moving_point = o.position();
}

void GLScene::finish_moving(){
    Point offset = m_moving_point - m_moving_start_point;
    BOOST_FOREACH(SimulationObject* o, m_moving_objects){
        SpatialObject* spo = dynamic_cast<SpatialObject*>(o);
        if(spo) spo->set_position(spo->position() + offset);
    }
    m_moving = false;
    updateGL();
}

void GLScene::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHT0);
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

    glShadeModel(GL_SMOOTH); // or GL_FLAT
    glEnable(GL_NORMALIZE); //or not
    glEnable(GL_LIGHTING);
}

void GLScene::resizeGL(int w, int h)
{
   // setup viewport, projection etc.:
   glViewport(0, 0, (GLint)w, (GLint)h);

   /*
   glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
   glLoadIdentity();							// Reset The Projection Matrix

   // Calculate The Aspect Ratio Of The Window
   gluPerspective(m_fov,(GLfloat)w/(GLfloat)h,0.1f,10000.0f);

   glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
   glLoadIdentity();							// Reset The Modelview Matrix
   */
}

void GLScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

    setup_projection_and_modelview_matrix();

    paint_floor();
    paint_objects();
    if(m_moving){
        paint_moving_plane();
        paint_objects(false,true);
    }


    glColor3f(1.f,1.f,1.f);
    renderText(300,0.,0.,QString("x"));
    renderText(0.,0.,300,QString("z"));

    //renderText(20.,20.,0.,QString("%1").arg(m_moving_start_point.x));
    //renderText(20.,40.,0.,QString("%1").arg(m_moving_start_point.y));
    //renderText(20.,60.,0.,QString("%1").arg(m_moving_start_point.z));

    std::list<QRect> regions_of_selected_objects;
    BOOST_FOREACH(SimulationObject* object, m_selected_objects){
        regions_of_selected_objects.push_back(occupied_2d_region_of_object(object));
    }


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();



    glClear(GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width(),0,height(),-100,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(m_selection_box)
        paint_selection_box();


    BOOST_FOREACH(QRect rect, regions_of_selected_objects){
        paint_selection_marker(rect);
    }


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glFlush();
    swapBuffers();
}

void GLScene::paint_selection_marker(QRect rect){
    glColor3f(1.,1.,0.);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glVertex2i(rect.left(),rect.top());
        glVertex2i(rect.left()+5,rect.top());
        glVertex2i(rect.left(),rect.top());
        glVertex2i(rect.left(),rect.top()-5);

        glVertex2i(rect.right(),rect.top());
        glVertex2i(rect.right()-5,rect.top());
        glVertex2i(rect.right(),rect.top());
        glVertex2i(rect.right(),rect.top()-5);

        glVertex2i(rect.left(),rect.bottom());
        glVertex2i(rect.left()+5,rect.bottom());
        glVertex2i(rect.left(),rect.bottom());
        glVertex2i(rect.left(),rect.bottom()+5);

        glVertex2i(rect.right(),rect.bottom());
        glVertex2i(rect.right()-5,rect.bottom());
        glVertex2i(rect.right(),rect.bottom());
        glVertex2i(rect.right(),rect.bottom()+5);
    glEnd();
}

void GLScene::setup_projection_and_modelview_matrix(){
    //Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fov, 1., 0.1, 10000.);

    //m_moving_start_point = mouse_on_plane(m_oldMouseX, m_oldMouseY, Point(0,0,0), Point(0,0,-1));
    //Set up modelview matrix
    apply_camera_translation_to_modelviewmatrix();
}

void GLScene::apply_camera_translation_to_modelviewmatrix(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(m_camera_config.center_position.x,m_camera_config.center_position.y,m_camera_config.center_position.z);
    glTranslatef(0.,0.,0.-m_camera_config.distance);
    glRotatef(m_camera_config.elevation, 1., 0., 0.);
    glRotatef(m_camera_config.azimuth, 0., 1., 0.);
}

void GLScene::paint_floor(){
    int step = 20;
    int size = 1000;
    glDisable(GL_LIGHTING);
    glColor3f(0.5f,0.5f,0.5f);
    glBegin(GL_LINES);
    for(float x = -size; x <= size; x += step){
        glVertex3i(x,0,-size);
        glVertex3i(x,0,size);
    }
    for(int z = -size; z <= size; z += step){
        glVertex3i(-size,0,z);
        glVertex3i(size,0,z);
    }
    glEnd();


}

void GLScene::paint_objects(bool picking_run, bool only_moving_objects){
    BOOST_FOREACH(SimulationObject* o, m_network->objects()){
        Neuron* neuron = dynamic_cast<Neuron*>(o);
        Link* link = dynamic_cast<Link*>(o);
        if(neuron){
            glPushMatrix();
            if(picking_run){
                glColor3ub(m_picking_names.size()+1,0,0);
                m_picking_names.push_back(neuron);
                glDisable(GL_DITHER);
                glDisable(GL_LIGHTING);
            }else{
                glEnable(GL_LIGHTING);
                glEnable(GL_DITHER);
                GLfloat green[] = {.2,1, std::max(0.,neuron->membrane_potential()+70.)/100.,1};
                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
            }

            if(only_moving_objects){
                if(!m_moving_objects.count(neuron)) continue;
                glEnable(GL_LIGHTING);
                glEnable(GL_DITHER);
                GLfloat transparent[] = {.9,.9,.9,0.5};
                glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transparent);
                Point offset = m_moving_point - m_moving_start_point;
                Point position = neuron->position() + offset;
                glTranslatef(position.x,position.y,position.z);
            }else{
                glTranslatef(neuron->position().x,neuron->position().y,neuron->position().z);
            }


            glutSolidSphere(15,20,20);
            glPopName();
            glPopMatrix();
        }

        if(link){

        }
    }
}

SimulationObject* GLScene::object_under_cursor(int cursorX, int cursorY) {
        makeCurrent();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setup_projection_and_modelview_matrix();
        m_picking_names.clear();
        paint_objects(true);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT,viewport);
        GLubyte pixel[3];
        pixel[0] = pixel[1] = pixel[2] = 0;
        glReadPixels(cursorX,viewport[3]-cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

        if (pixel[0]>0)
            return m_picking_names[pixel[0]-1];
        else return 0;

}

Point GLScene::mouse_on_plane(int x, int y, Point plane_origin, Point normal){
    makeCurrent();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    y = viewport[3]-y;
    GLdouble model_view[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
    GLdouble projection[16];glGetDoublev(GL_PROJECTION_MATRIX, projection);
    Point a,b;
    //GLfloat depth[2];
    //glReadPixels (x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
    gluUnProject(x, y, 0, model_view, projection, viewport, &a.x, &a.y, &a.z);
    gluUnProject(x, y, 1, model_view, projection, viewport, &b.x, &b.y, &b.z);

    Point ray = b-a;
    //gerade: camera + n*ray
    //n, bei dem die gerade die ebene schneidet:
    Point cam = m_camera_config.position();

    double zaehler = normal * (plane_origin - cam);
    double nenner  = normal * ray;
    double n = zaehler / nenner;

    //double help = cam.x;
    //cam.x = cam.z;
    //cam.z = help;

    Point schnittpunkt = cam + ray*n;
    return schnittpunkt;
}

QRect GLScene::occupied_2d_region_of_object(SimulationObject* object){
    makeCurrent();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    GLdouble model_view[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    Neuron* neuron = dynamic_cast<Neuron*>(object);
    if(neuron){
        double x = neuron->position().x, y = neuron->position().y, z = neuron->position().z;
        GLdouble x2d, y2d, z2d;
        gluProject(x + 20, y, z, model_view, projection, viewport, &x2d, &y2d, &z2d);

        double left = x2d, right = x2d, top = y2d, bottom = y2d;

        gluProject(x - 20, y, z, model_view, projection, viewport, &x2d, &y2d, &z2d);
        if(x2d<left)left=x2d;
        if(x2d>right)right=x2d;
        if(y2d<bottom)bottom=y2d;
        if(y2d>top)top=y2d;
        gluProject(x, y + 20, z, model_view, projection, viewport, &x2d, &y2d, &z2d);
        if(x2d<left)left=x2d;
        if(x2d>right)right=x2d;
        if(y2d<bottom)bottom=y2d;
        if(y2d>top)top=y2d;
        gluProject(x, y - 20, z, model_view, projection, viewport, &x2d, &y2d, &z2d);
        if(x2d<left)left=x2d;
        if(x2d>right)right=x2d;
        if(y2d<bottom)bottom=y2d;
        if(y2d>top)top=y2d;
        gluProject(x, y, z + 20, model_view, projection, viewport, &x2d, &y2d, &z2d);
        if(x2d<left)left=x2d;
        if(x2d>right)right=x2d;
        if(y2d<bottom)bottom=y2d;
        if(y2d>top)top=y2d;
        gluProject(x, y, z - 20, model_view, projection, viewport, &x2d, &y2d, &z2d);
        if(x2d<left)left=x2d;
        if(x2d>right)right=x2d;
        if(y2d<bottom)bottom=y2d;
        if(y2d>top)top=y2d;

        return QRect(left,top,right-left,bottom-top);
    }

    return QRect();
}


Point GLScene::CameraConfig::position(){
    double pi = 3.141592653589793;
    double x = -cos(elevation/360*2*pi)*sin(azimuth/360*2*pi);
    double y = sin(elevation/360*2*pi);
    double z = cos(elevation/360*2*pi)*cos(azimuth/360*2*pi);
    Point camera_offset = Point(x,y,z)*distance;
    return center_position + camera_offset;
}

void GLScene::paint_selection_box(){
    int left, right, up, down;
    left = m_selection_box_origin[0] < m_selection_box_current[0] ? m_selection_box_origin[0] : m_selection_box_current[0];
    right = m_selection_box_origin[0] > m_selection_box_current[0] ? m_selection_box_origin[0] : m_selection_box_current[0];
    up = m_selection_box_origin[1] < m_selection_box_current[1] ? m_selection_box_origin[1] : m_selection_box_current[1];
    down = m_selection_box_origin[1] > m_selection_box_current[1] ? m_selection_box_origin[1] : m_selection_box_current[1];
    up = height() - up;
    down = height() -down;
    glColor3f(1.,1.,0.);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_STRIP);
        glVertex3i(left,up,-1);
        glVertex3i(left,down,-1);
        glVertex3i(right,down,-1);
        glVertex3i(right,up,-1);
        glVertex3i(left,up,-1);
    glEnd();
}

std::set<SimulationObject*> GLScene::objects_in_selection_box(){
    makeCurrent();
    int left, right, up, down;
    left = m_selection_box_origin[0] < m_selection_box_current[0] ? m_selection_box_origin[0] : m_selection_box_current[0];
    right = m_selection_box_origin[0] > m_selection_box_current[0] ? m_selection_box_origin[0] : m_selection_box_current[0];
    up = m_selection_box_origin[1] < m_selection_box_current[1] ? m_selection_box_origin[1] : m_selection_box_current[1];
    down = m_selection_box_origin[1] > m_selection_box_current[1] ? m_selection_box_origin[1] : m_selection_box_current[1];
    up = height() - up;
    down = height() -down;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setup_projection_and_modelview_matrix();
    m_picking_names.clear();
    paint_objects(true);

    int width = right-left;
    int height = up-down;
    glReadPixels(left,down,width,height,GL_RGB,GL_UNSIGNED_BYTE,(void *)m_pixel_buffer);


    std::set<SimulationObject*> result;
    for(int x=0;x<width*height*3;x+=3){
        if(m_pixel_buffer[x] > 0){
            SimulationObject* o = m_picking_names[m_pixel_buffer[x]-1];
            result.insert(o);
            //GLubyte bla = m_pixel_buffer[x];
            //GLubyte a = bla;
            //bla = a;
        }
    }

    return result;
}

void GLScene::paint_moving_plane(){
    Point p = m_moving_switch_plane_point;
    Point destination = m_moving_point;
    double dist = destination.distance(p);
    glDisable(GL_LIGHTING);
    glColor3f(.0,.0,.8);
    for(double step=10;step < std::max(2*dist,150.);step*=1.5){
        if(!m_shift_key_down && !m_ctrl_key_down){
            //parallel to x-z-achsis
            glBegin(GL_LINE_STRIP);
                glVertex3f(p.x - step, p.y, p.z - step);
                glVertex3f(p.x + step, p.y, p.z - step);
                glVertex3f(p.x + step, p.y, p.z + step);
                glVertex3f(p.x - step, p.y, p.z + step);
                glVertex3f(p.x - step, p.y, p.z - step);
            glEnd();

        }else if(m_shift_key_down && !m_ctrl_key_down){
            //parallell to x-y-achsis
            glBegin(GL_LINE_STRIP);
                glVertex3f(p.x - step, p.y - step, p.z);
                glVertex3f(p.x + step, p.y - step, p.z);
                glVertex3f(p.x + step, p.y + step, p.z);
                glVertex3f(p.x - step, p.y + step, p.z);
                glVertex3f(p.x - step, p.y - step, p.z);
            glEnd();
        }else if(m_ctrl_key_down && !m_shift_key_down){
            //parallel to y-z-achsis
            glBegin(GL_LINE_STRIP);
                glVertex3f(p.x, p.y - step, p.z - step);
                glVertex3f(p.x, p.y - step, p.z + step);
                glVertex3f(p.x, p.y + step, p.z + step);
                glVertex3f(p.x, p.y + step, p.z - step);
                glVertex3f(p.x, p.y - step, p.z - step);
            glEnd();
        }

    }

}


void GLScene::add_to_selection(SimulationObject* o){
    m_selected_objects.insert(o);
    emit selection_changed(m_selected_objects);
}

void GLScene::select(SimulationObject* o){
    m_selected_objects.clear();
    if(o) m_selected_objects.insert(o);
    emit selection_changed(m_selected_objects);
    if(o){
        Neuron* n = dynamic_cast<Neuron*>(o);
        if(n) emit neuron_selected(n);
    }else emit neuron_selected(0);

}

void GLScene::select(std::set<SimulationObject*> o){
    m_selected_objects = o;
    emit selection_changed(m_selected_objects);
}

