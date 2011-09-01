#include "glscene.h"
#include <QMouseEvent>
#include <boost/foreach.hpp>
#include "neuron.h"
#include "axon.h"

#include <algorithm>
#include "application.h"
#include <assert.h>
#include "drawable.h"
#include "drawableneuron.h"
#include "drawableaxonnode.h"
#include "drawabledendritenode.h"
#include "drawablecurrentinducer.h"
#include "drawablelineardiscriminator.h"
#include "synapse.h"
#include "drawablesynapse.h"
#include "current_inducer.h"
#include "samples.h"
#include "drawablesamples.h"
#include "menuobjectrightclick.h"
#include "lineardiscriminator.h"
#include "ldconnection.h"

GLScene::GLScene(QWidget *parent) :
    QGLWidget(parent),
    m_network(0),
    m_mousedown_right(false), m_mousedown_left(false),
    m_shift_key_down(false), m_ctrl_key_down(false),
    m_selection_box(false),
    m_moving(false),
    m_moving_start_point(0,0,0),
    m_connecting(false),
    m_connection_source(0),
    m_current_connection_target(0),
    m_fov(120.),
    m_inserting_current_inducer(false),
    m_phony_current_inducer(0)
{
    m_camera_config.distance = 100;
    setMouseTracking(true);
    setAutoBufferSwap(false);
    m_pixel_buffer = new GLubyte[2000*2000];
    m_camera_config.elevation = m_camera_config.azimuth = 45.;

    qRegisterMetaType< std::set<SimulationObject*> >("std::set<SimulationObject*>");

    connect(&Application::instance(), SIGNAL(refresh()), this, SLOT(updateGL()));
    connect(&m_camera_center_moving_timer, SIGNAL(timeout()), this, SLOT(camera_center_moving_update()));

    BOOST_FOREACH(Drawable* d, Drawables::instance().get_all_drawables()){
        d->reset_display_lists();
    }
    m_last_paint_time.start();
    m_currently_painting = false;
}

GLScene::~GLScene(){
    delete[] m_pixel_buffer;
}

void GLScene::set_network(Network* network){
    if(m_network) disconnect(m_network, SIGNAL(object_deleted(SimulationObject*)), this, SLOT(object_deleted(SimulationObject*)));
    m_network = network;
    connect(m_network, SIGNAL(object_deleted(SimulationObject*)), this, SLOT(object_deleted(SimulationObject*)));
}

std::set<SimulationObject*> GLScene::selected_objects(){
    return m_selected_objects;
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
                //updateGL();
        }


        if(m_mousedown_left && !m_moving){
            SimulationObject* clicked_object = 0;
                if(m_selection_box){
                    m_selection_box_current[0] = e->x();
                    m_selection_box_current[1] = e->y();
                }else{
                    clicked_object = object_under_cursor(e->x(),e->y());
                    if(clicked_object){
                        //movable object dragged -> start moving:
                        if(!m_selected_objects.count(clicked_object))
                            m_selected_objects.clear();
                        if(m_selected_objects.empty())
                            add_to_selection(clicked_object);

                        start_moving(*clicked_object);
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

        if(m_moving){
            Point normal(0,1,0);
            if(m_shift_key_down) normal = Point(0,0,1);
            if(m_ctrl_key_down) normal = Point(1,0,0);
            m_moving_point = mouse_on_plane(e->x(),e->y(),m_moving_switch_plane_point, normal);
        }

        if(m_connecting){
            assert(m_connection_source);
            if(dynamic_cast<AxonNode*>(m_connection_source)){
                std::pair<SimulationObject*,double> axon_node = find_nearest_2d<AxonNode*>(e->x(),e->y());
                std::pair<SimulationObject*,double> dendritic_node = find_nearest_2d<DendriticNode*>(e->x(),e->y());
                if(axon_node.first) m_current_connection_target = axon_node.first;
                if(dendritic_node.first) m_current_connection_target = dendritic_node.first;
                if(axon_node.first && dendritic_node.first)
                    if(axon_node.second < dendritic_node.second) m_current_connection_target = axon_node.first;
            }
            if(dynamic_cast<Samples*>(m_connection_source)){
                m_current_connection_target = find_nearest_2d<CurrentInducer*>(e->x(),e->y()).first;
            }
            if(dynamic_cast<LinearDiscriminator*>(m_connection_source)){
                m_current_connection_target = find_nearest_2d<LinearDiscriminator*>(e->x(),e->y()).first;
            }
        }

        if(m_inserting_current_inducer){
            assert(m_phony_current_inducer);
            SimulationObject* dn = find_nearest_2d<DendriticNode*>(e->x(),e->y()).first;
            if(dn){
                m_phony_current_inducer->set_position(dn->position());
            }
        }

        m_oldMouseX = e->x();
        m_oldMouseY = e->y();
        //updateGL();
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
                    if(o){
                        if(m_shift_key_down)
                            add_to_selection(o);
                        else
                            select(o);
                    }
                }
                if(m_connecting){
                    finish_connecting();
                }
                if(m_inserting_current_inducer){
                    finish_inserting_current_inducer(find_nearest_2d<DendriticNode*>(e->x(),e->y()).first);
                }
                //updateGL();
            }
        }

        if(e->button() == Qt::RightButton){
            SimulationObject* o = object_under_cursor(e->x(),e->y());
            if(o){
                if(m_selected_objects.size() > 1){
                    emit right_click_on_object_during_multiselection(e->pos());
                }else{
                    std::list<std::string> actions = o->user_actions();
                    if(actions.size() > 0){
                        MenuObjectRightClick menu(o);
                        menu.popup(mapToGlobal(e->pos()));
                        menu.exec();
                    }
                }
            }

        }
}

void GLScene::mouseDoubleClickEvent(QMouseEvent *e){
    SimulationObject* object = object_under_cursor(e->x(),e->y());
    if(!object) return;

    m_camera_center_moving_target = object->position();
    m_camera_center_moving_source = m_camera_config.center_position;
    m_camera_center_moving_param = 0;
    m_camera_center_moving_timer.start(10);
    //m_camera_config.center_position = object->position();
}


void GLScene::wheelEvent(QWheelEvent *e){
    setFocus();
        m_camera_config.distance -= e->delta()*pow(2,m_camera_config.distance/10000.f);
        if( m_camera_config.distance<70) m_camera_config.distance=70;
        if( m_camera_config.distance>10000) m_camera_config.distance=10000;
        //updateGL();
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
        //if(m_moving) updateGL();
        break;
    case Qt::Key_Control:
        m_ctrl_key_down = true;
        m_moving_switch_plane_point = m_moving_point;
        //if(m_moving) updateGL();
        break;
    case Qt::Key_Escape:
        abort_moving();
        m_connecting = false;
        finish_inserting_current_inducer(0);
        //updateGL();
        break;
    }

    //updateGL();

    e->accept();
}

void GLScene::keyReleaseEvent(QKeyEvent *e){
    switch(e->key()){
    case Qt::Key_Shift:
        m_shift_key_down = false;
        m_moving_switch_plane_point = m_moving_point;
        //if(m_moving) updateGL();
        break;
    case Qt::Key_Control:
        m_ctrl_key_down = false;
        m_moving_switch_plane_point = m_moving_point;
        //if(m_moving) updateGL();
        break;
    }
}

void GLScene::start_moving(const SimulationObject& o){
    m_moving_objects.clear();
    BOOST_FOREACH(SimulationObject* object,m_selected_objects){
        if(!object->is_user_movable()) continue;
        m_moving_objects.insert(object);
    }
    m_moving = true;
    m_moving_switch_plane_point = m_moving_start_point = m_moving_point = o.position() + o.moving_offset();
}

void GLScene::start_inserting(std::set<SimulationObject*> objects){
    //this is to make sure that all child objects get moved together with toplevel objects:
    std::set<SimulationObject*> children;
    BOOST_FOREACH(SimulationObject* o, objects){
        BOOST_FOREACH(SimulationObject* child, o->children()){
            children.insert(child);
        }
    }
    BOOST_FOREACH(SimulationObject* o, children){
        objects.insert(o);
    }



    m_moving_objects = objects;
    m_moving = m_insert_moving = true;
    m_moving_switch_plane_point = m_moving_start_point = m_moving_point = m_camera_config.center_position;

    BOOST_FOREACH(SimulationObject* o, objects){
        o->set_position(o->position()+m_camera_config.center_position - o->moving_offset());
    }
}

void GLScene::finish_moving(){
    Point offset = m_moving_point - m_moving_start_point;

    BOOST_FOREACH(SimulationObject* o, m_moving_objects){
        if(m_insert_moving){
            m_network->add_object(o);
        }

        if(!o->is_user_movable()) continue;
        o->set_position(o->position() + offset);// - o->moving_offset());
    }

    m_moving = false;
    m_insert_moving = false;
    m_moving_objects.clear();
    emit user_interaction();
    //updateGL();
}

void GLScene::abort_moving(){
    m_moving = false;
    if(m_insert_moving){
        BOOST_FOREACH(SimulationObject* o, m_moving_objects){
            delete o;
        }
    }
    m_insert_moving = false;
    m_moving_objects.clear();
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

    if(m_currently_painting || m_last_paint_time.elapsed() < 30) return;
    m_currently_painting = true;
    m_last_paint_time.restart();


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

    setup_projection_and_modelview_matrix();

    paint_floor();
    if(m_moving) paint_moving_plane();
    if(m_connecting) paint_connecting_overlay();
    paint_objects();



    //glColor3f(1.f,1.f,1.f);
    //renderText(300,0.,0.,QString("x"));
    //renderText(0.,0.,300,QString("z"));

    //renderText(20.,20.,0.,QString("%1").arg(m_moving_start_point.x));
    //renderText(20.,40.,0.,QString("%1").arg(m_moving_start_point.y));
    //renderText(20.,60.,0.,QString("%1").arg(m_moving_start_point.z));




    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();


    std::list<QRect> regions_of_selected_objects;
    BOOST_FOREACH(SimulationObject* object, m_selected_objects){
        if(!object) continue;
        regions_of_selected_objects.push_back(occupied_2d_region_of_object(object));
    }


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
    m_currently_painting = false;
}

void GLScene::paint_connecting_overlay(){
    if(!m_connection_source || !m_current_connection_target) return;
    glColor3f(0.,1.,0.);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glVertex3f(m_connection_source->position().x, m_connection_source->position().y, m_connection_source->position().z);
        glVertex3f(m_current_connection_target->position().x, m_current_connection_target->position().y, m_current_connection_target->position().z);
    glEnd();
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
    gluPerspective(m_fov, static_cast<double>(width())/height(), 0.1, 10000.);

    //m_moving_start_point = mouse_on_plane(m_oldMouseX, m_oldMouseY, Point(0,0,0), Point(0,0,-1));
    //Set up modelview matrix
    apply_camera_translation_to_modelviewmatrix();
}

void GLScene::apply_camera_translation_to_modelviewmatrix(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.,0.,0.-m_camera_config.distance);
    glRotatef(m_camera_config.elevation, 1., 0., 0.);
    glRotatef(m_camera_config.azimuth, 0., 1., 0.);

    glTranslatef(-m_camera_config.center_position.x,-m_camera_config.center_position.y,-m_camera_config.center_position.z);
}

void GLScene::paint_floor(){
    int step = 20;
    int size = 1000;
    glDisable(GL_LIGHTING);
    glColor3f(0.5f,0.5f,0.5f);
    glLineWidth(1);
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

void GLScene::paint_objects(bool picking_run){
    if(!picking_run){
        BOOST_FOREACH(SimulationObject* o, m_moving_objects){
            paint_object(o,false,true);
         }
    }
    BOOST_FOREACH(SimulationObject* o, m_network->objects_as_std_set()){
        paint_object(o,picking_run);
    }
}

void GLScene::paint_object(SimulationObject* o, bool picking, bool moving){

    if(!moving)     //child objects are not moved when parent is moved!
    BOOST_FOREACH(SimulationObject* child, o->children()){
        paint_object(child, picking, moving);
    }

    if(o->bad_hacks[GLScene::DRAWABLE_POINTER] == 0){
        BOOST_FOREACH(Drawable* drawable, Drawables::instance().get_all_drawables()){
            if(!drawable->is_applicable_to(o)) continue;
            o->bad_hacks[GLScene::DRAWABLE_POINTER] = drawable;
        }
    }
    Drawable* drawable = static_cast<Drawable*>(o->bad_hacks[GLScene::DRAWABLE_POINTER]);
    if(!drawable) return;
    drawable->init_with_object(o);

    glPushMatrix();

    Point pos = o->position();
    if(moving){
        Point offset = m_moving_point - m_moving_start_point;
        pos += offset;// - o->moving_offset();
    }

    if(drawable->do_translate_bevor_drawing() || moving){
        glTranslatef(pos.x, pos.y, pos.z);
    }

    if(picking) drawable->draw_picking();
    else if(moving) drawable->draw_moving();
    else drawable->draw();


    glPopMatrix();
}


SimulationObject* GLScene::object_under_cursor(int cursorX, int cursorY) {
        makeCurrent();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setup_projection_and_modelview_matrix();
        //m_picking_names.clear();
        paint_objects(true);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT,viewport);
        GLubyte pixel[3];
        pixel[0] = pixel[1] = pixel[2] = 0;
        glReadPixels(cursorX,viewport[3]-cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

        return Drawable::object_for_picking_name(boost::make_tuple(pixel[0],pixel[1],pixel[2]));
        //if (pixel[0]>0)
        //    return m_picking_names[pixel[0]-1];
        //else return 0;

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

    /*
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setup_projection_and_modelview_matrix();
    m_picking_names.clear();
    paint_object(object, true);

    glReadPixels(0,0,width(),height(),GL_RGB,GL_UNSIGNED_BYTE,(void *)m_pixel_buffer);


    int minx=width(),miny=height(),maxx=-1,maxy=-1;
    for(int y=0;y<height();y++){
        for(int x=0;x<width();x++){
            int number = m_pixel_buffer[(x + width()*y)*3];

            if(number <= 0) continue;
            //SimulationObject* o = m_picking_names[number-1];
            //if(o != object) continue;
            int realy = height() - y;
            if(x < minx) minx=x;
            if(x > maxx) maxx=x;
            if(realy < miny) miny=realy;
            if(realy > maxy) maxy=realy;

        }
    }

    maxy = height() - maxy;
    miny = height() - miny;
    //maxx -= width()/2;
    //minx -= width()/2;

    minx -= 100;
    maxx -= 100;

    return QRect(minx,miny,maxx - minx,maxy - miny);;

*/








    makeCurrent();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    GLdouble model_view[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    Neuron* neuron = dynamic_cast<Neuron*>(object);
    Axon* axon = dynamic_cast<Axon*>(object);
    AxonNode* axon_node = dynamic_cast<AxonNode*>(object);
    DendriticNode* dendritic_node = dynamic_cast<DendriticNode*>(object);
    Synapse* synapse = dynamic_cast<Synapse*>(object);
    CurrentInducer* current_inducer = dynamic_cast<CurrentInducer*>(object);
    Samples* samples = dynamic_cast<Samples*>(object);
    LinearDiscriminator* linear_discriminator = dynamic_cast<LinearDiscriminator*>(object);
    LDConnection* ld_connection = dynamic_cast<LDConnection*>(object);
    //SpatialObject* spo = dynamic_cast<SpatialObject*>(object);
    //assert(spo);

    GLdouble start_x, start_y, start_z;
    GLdouble end_x, end_y, end_z;

    if(axon){
        GLdouble x = axon->emitter()->position().x, y = axon->emitter()->position().y, z = axon->emitter()->position().z;
        gluProject(x, y, z, model_view, projection, viewport, &start_x, &start_y, &start_z);
        x = axon->receiver()->position().x; y = axon->receiver()->position().y; z = axon->receiver()->position().z;
        gluProject(x, y, z, model_view, projection, viewport, &end_x, &end_y, &end_z);
    }

    if(ld_connection){
        GLdouble x = ld_connection->pre_neuron()->position().x, y = ld_connection->pre_neuron()->position().y, z = ld_connection->pre_neuron()->position().z;
        gluProject(x, y, z, model_view, projection, viewport, &start_x, &start_y, &start_z);
        x = ld_connection->post_neuron()->position().x; y = ld_connection->post_neuron()->position().y; z = ld_connection->post_neuron()->position().z;
        gluProject(x, y, z, model_view, projection, viewport, &end_x, &end_y, &end_z);
    }

    if(axon || ld_connection){
        int left,right,top,bottom;
        if(start_x < end_x){
            left = start_x; right = end_x;
        }else{
            left = end_x; right = start_x;
        }
        if(start_y > end_y){
            top = start_y; bottom = end_y;
        }else{
            top = end_y; bottom = start_y;
        }

        return QRect(left, top, right - left, bottom - top);
    }

    double x, y, z;

    double radius;
    Point displacement(0,0,0);
    if(neuron){
        radius = DrawableNeuron::SIZE+3;
    }
    if(axon_node){
        radius = DrawableAxonNode::SIZE+3;
    }
    if(dendritic_node){
        radius = DrawableDendriteNode::SIZE+3;
    }
    if(synapse){
        displacement = DrawableSynapse::displacement(synapse);
        radius = (DrawableSynapse::START_DISTANCE - DrawableSynapse::END_DISTANCE)/2 + 3;
    }
    if(current_inducer){
        double r = (DrawableCurrentInducer::HEIGHT2-DrawableCurrentInducer::HEIGHT1)/2 + 3;
        displacement = Point(0,r,-3);
        radius = r;
    }
    if(samples){
        radius = DrawableSamples::SIZE + 3;
    }
    if(linear_discriminator){
        radius = DrawableLinearDiscriminator::SIZE + 3;
    }

    Point position = object->position() + displacement;
    x = position.x; y = position.y; z = position.z;


    GLdouble x1,x2,y1,y2,z1,z2;
    gluUnProject(width()/2, height()/2, 0, model_view, projection, viewport, &x1, &y1, &z1);
    gluUnProject(width()/2+1, height()/2, 0, model_view, projection, viewport, &x2, &y2, &z2);
    Point rechts(x2-x1,y2-y1,z2-z1);
    gluUnProject(width()/2, height()/2+1, 0, model_view, projection, viewport, &x2, &y2, &z2);
    Point hoch(x2-x1,y2-y1,z2-z1);
    rechts = rechts / rechts.length();
    rechts *= radius;
    hoch = hoch / hoch.length();
    hoch *= radius;

    GLdouble mitte_x, mitte_y, mitte_z;
    gluProject(x, y, z, model_view, projection, viewport, &mitte_x, &mitte_y, &mitte_z);
    GLdouble rechts_x, rechts_y, rechts_z;
    gluProject(x + rechts.x, y + rechts.y, z + rechts.z, model_view, projection, viewport, &rechts_x, &rechts_y, &rechts_z);
    GLdouble links_x, links_y, links_z;
    gluProject(x - rechts.x, y - rechts.y, z - rechts.z, model_view, projection, viewport, &links_x, &links_y, &links_z);
    GLdouble oben_x, oben_y, oben_z;
    gluProject(x + hoch.x, y + hoch.y, z + hoch.z, model_view, projection, viewport, &oben_x, &oben_y, &oben_z);
    GLdouble unten_x, unten_y, unten_z;
    gluProject(x - hoch.x, y - hoch.y, z - hoch.z, model_view, projection, viewport, &unten_x, &unten_y, &unten_z);

    return QRect(links_x,oben_y,rechts_x - links_x,unten_y - oben_y);

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
    glLineWidth(1);
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
    paint_objects(true);

    int width = right-left;
    int height = up-down;
    glReadPixels(left,down,width,height,GL_RGB,GL_UNSIGNED_BYTE,(void *)m_pixel_buffer);


    std::set<SimulationObject*> result;
    for(int x=0;x<width*height*3;x+=3){
        GLuint r = m_pixel_buffer[x];
        GLuint g = m_pixel_buffer[x+1];
        GLuint b = m_pixel_buffer[x+2];
        SimulationObject* o = Drawable::object_for_picking_name(boost::make_tuple(r,g,b));
        if(o) result.insert(o);
    }

    return result;
}

void GLScene::paint_moving_plane(){
    Point p = m_moving_switch_plane_point;
    Point destination = m_moving_point;
    double dist = destination.distance(p);
    glDisable(GL_LIGHTING);
    glColor3f(.0,.0,.8);
	double my_max;
#ifdef WIN32
	my_max = _cpp_max(2*dist,150.);
#else
	my_max = std::max(2*dist,150.);
#endif

    for(double step=10;step < my_max;step*=1.5){
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

void GLScene::deselect(){
    m_selected_objects.clear();
    emit selection_changed(m_selected_objects);
}

void GLScene::object_deleted(SimulationObject* object){    
    m_moving_objects.erase(object);
    if(m_selected_objects.count(object)){
        m_selected_objects.erase(object);
        emit selection_changed(m_selected_objects);
    }
}

void GLScene::camera_center_moving_update(){
    if(m_camera_center_moving_param>=1){
        m_camera_center_moving_timer.stop();
        m_camera_config.center_position = m_camera_center_moving_target;
        return;
    }

    double pi = 3.141592653589793;
    double fac = (sin(m_camera_center_moving_param*pi - pi/2) + 1) /2;
    m_camera_center_moving_param += 0.02;

    m_camera_config.center_position = m_camera_center_moving_source + (m_camera_center_moving_target - m_camera_center_moving_source)*fac;

}


void GLScene::start_connecting(SimulationObject* emitter){
    m_connection_source = emitter;
    m_connecting = true;
}

void GLScene::finish_connecting(){
    SpikingObject* spike_receiver = dynamic_cast<SpikingObject*>(m_current_connection_target);
    DendriticNode* dendritic_node = dynamic_cast<DendriticNode*>(m_current_connection_target);
    CurrentInducer* current_inducer = dynamic_cast<CurrentInducer*>(m_current_connection_target);
    LinearDiscriminator* ld_target = dynamic_cast<LinearDiscriminator*>(m_current_connection_target);

    AxonNode* axon_node = dynamic_cast<AxonNode*>(m_connection_source);
    if(axon_node){
        if(spike_receiver && (axon_node != spike_receiver)) m_network->connect(axon_node, spike_receiver);
        if(dendritic_node) m_network->connect(axon_node, dendritic_node);
    }

    Samples* samples = dynamic_cast<Samples*>(m_connection_source);
    if(samples && current_inducer) samples->add_current_inducer(current_inducer);

    LinearDiscriminator* ld_source = dynamic_cast<LinearDiscriminator*>(m_connection_source);
    if(ld_source) m_network->connect(ld_source, ld_target);

    m_connecting = false;
}

void GLScene::start_inserting_current_inducer(){
    m_phony_current_inducer = new CurrentInducer(m_network->simulation());
    m_inserting_current_inducer = true;
    m_moving_point = m_moving_start_point = Point(0,0,0);
    m_moving_objects.insert(m_phony_current_inducer);
}

void GLScene::finish_inserting_current_inducer(SimulationObject* o){
    BOOST_FOREACH(SimulationObject* mo, m_moving_objects){
        delete mo;
    }
    m_moving_objects.clear();
    m_inserting_current_inducer = false;
    m_phony_current_inducer = false;

    DendriticNode* dn = dynamic_cast<DendriticNode*>(o);
    if(!dn) return;

    m_network->add_object(new CurrentInducer(dn));
}
