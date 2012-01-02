#include "group.h"
#include "simulation.h"
#include <boost/foreach.hpp>
#include "userinteractionadapter.h"
#include "lineardiscriminator.h"
#include "network.h"
#include "neuron.h"
#include "RandomGenerator.h"
#include "math_constants.h"
#include "ldconnection.h"

Group::Group(Simulation* sim)
    : SimulationObject(sim), m_update_time_interval(0.), m_elapsed_sim_time(0.), m_drawn_horizontally(true),
      m_backprop_target(0), m_mlp_output(0)
{
}



void Group::update(double milli_seconds){
    m_elapsed_sim_time += milli_seconds;
    if(m_elapsed_sim_time < m_update_time_interval) return;

    m_elapsed_sim_time -= m_update_time_interval;
}

void Group::create_mlp(unsigned int number_of_layers, std::vector<int> number_of_neurons){
    const double SPACING = 50.;
    assert(number_of_neurons.size() == number_of_layers);
    double x = position().x - SPACING;
    double y = position().y;
    std::set<LinearDiscriminator*> last_layer;
    std::set<LinearDiscriminator*> current_layer;
    for(unsigned int i=0;i < number_of_layers; i++){
        double z = position().z;
        unsigned int neurons = number_of_neurons[i];
        z -= neurons/2. * SPACING;
        for(unsigned int neuron_index = 0; neuron_index < neurons; neuron_index++){
            LinearDiscriminator* ld = new LinearDiscriminator(simulation());
            ld->set_position(Point(x,y,z));
            add_object(ld);
            current_layer.insert(ld);
            z += SPACING;
        }
        x += SPACING*2;
        if(i>0){
            BOOST_FOREACH(LinearDiscriminator* last, last_layer){
                BOOST_FOREACH(LinearDiscriminator* current, current_layer){
                    simulation()->network()->connect(last,current);
                }
            }
        }
        last_layer = current_layer;
        current_layer.clear();
    }
    set_drawn_horizontally(true);
}

void Group::create_2d_grid(unsigned int width, unsigned int height, double distance){
    Point center = position();
    for(int x=0;x<width;x++){
        for(int y=0;y<height;y++){
            Point neuron_pos = center;
            neuron_pos += Point(0,(y-static_cast<int>(height)/2)*distance,(x-static_cast<int>(width)/2)*distance);
            Neuron* neuron = new Neuron(simulation(), neuron_pos);
            m_objects.insert(neuron);
            simulation()->network()->add_object(neuron);
        }
    }
    set_drawn_horizontally(false);
}

void Group::create_connections(double dist1){
    double alpha = 0.693147 / dist1; //ln(0.5)
    std::set<Neuron*> local_neurons = neurons();
    BOOST_FOREACH(Neuron* n1, local_neurons){
        BOOST_FOREACH(Neuron* n2, local_neurons){
            if(n1==n2) continue;
            double distance = n1->position().distance(n2->position());
            double rand = NeuroMath::RandomGenerator::getInstance()->uniform(0,1);
            double e = pow(NeuroMath::e(), -distance * alpha);
            if(rand < e){
                simulation()->network()->connect(n1->axon_root(), n2->dendrites_root());
            }
        }
    }
}

void Group::set_synapse_weights(double mean, double var){
    std::set<Synapse*> all_synapses;
    std::set<LDConnection*> all_ld_connections = extract_all<LDConnection>(m_objects);
    BOOST_FOREACH(Neuron* n, neurons()){
        BOOST_FOREACH(Synapse* synapse, n->incoming_synapses()){
            all_synapses.insert(synapse);
        }
    }
    BOOST_FOREACH(LinearDiscriminator* ld, linear_discriminators()){
        BOOST_FOREACH(LDConnection* conn, ld->inputs()){
            all_ld_connections.insert(conn);
        }
    }

    BOOST_FOREACH(Synapse* synapse, all_synapses){
        synapse->set_weight(NeuroMath::RandomGenerator::getInstance()->gauss(mean, var));
    }
    BOOST_FOREACH(LDConnection* synapse, all_ld_connections){
        synapse->set_weight(NeuroMath::RandomGenerator::getInstance()->gauss(mean, var));
    }
}

void Group::connect_input_neuron_with_all(SpikingObject* input){
    BOOST_FOREACH(Neuron* n, neurons()){
        simulation()->network()->connect(input, n->dendrites_root());
    }
}

void Group::connect_input_neuron_randomly(SpikingObject* input, unsigned int count){
    std::set<unsigned int> indices;
    std::set<Neuron*> local_neurons = neurons();
    std::vector<Neuron*> neurons(local_neurons.begin(),local_neurons.end());
    while(indices.size() < count){
        unsigned int rand = static_cast<unsigned int>(NeuroMath::RandomGenerator::getInstance()->uniform(1,indices.size()+1));
        indices.insert(rand);
    }
    BOOST_FOREACH(unsigned int index, indices){
        simulation()->network()->connect(input, neurons[index]->dendrites_root());
    }
}


std::list<std::string> Group::user_actions(){
    std::list<std::string> actions;
    actions.push_back("Create MLP...");
    actions.push_back("Create LSM Column...");
    actions.push_back("Create connections...");
    actions.push_back("Set synapse weights...");
    actions.push_back("Do Backpropagation");
    return actions;
}

std::set<std::string> Group::active_user_actions(){
    std::set<std::string> s;
    if(m_objects.size() == 0){
        s.insert("Create MLP...");
        s.insert("Create LSM Column...");
    }else{
        if(neurons().size() > 0){
            s.insert("Create connections...");
            s.insert("Set synapse weights...");
        }
        if(linear_discriminators().size() > 0){
            s.insert("Set synapse weights...");
            if(mlp_output() && backprop_target())
                s.insert("Do Backpropagation");
        }
    }
    return s;
}

void Group::do_user_action(std::string action){
    if("Create MLP..." == action){
        std::vector<std::string> names;
        names.push_back("Number of layers");
        std::vector<std::pair<int,int> > limits;
        limits.push_back(std::pair<int,int>(0,std::numeric_limits<int>::max()));
        std::vector<int> values = UserInteractionAdapter::instance()->get_integer_values(names, "Group "+objectName().toStdString(), "Parameters for MLP", limits);
        if(values.size() != 1) return;

        unsigned int number_of_layers = values[0];
        names.clear();
        limits.clear();
        for(unsigned int i=0;i<number_of_layers;i++){
            names.push_back(QString("Number of neurons in layer %1").arg(i).toStdString());
            limits.push_back(std::pair<int,int>(1,std::numeric_limits<int>::max()));
        }
        values = UserInteractionAdapter::instance()->get_integer_values(names, "Group "+objectName().toStdString(), "Parameters for MLP", limits);
        if(values.size() != number_of_layers) return;
        create_mlp(number_of_layers,values);
    }
    if("Create LSM Column..." == action){
        std::vector<string> names;
        names.push_back("Width");
        names.push_back("Height");
        names.push_back("Distance");
        std::vector<std::pair<double,double> > limits;
        limits.push_back(std::pair<double,double>(0,std::numeric_limits<double>::max()));
        limits.push_back(std::pair<double,double>(0,std::numeric_limits<double>::max()));
        limits.push_back(std::pair<double,double>(10,std::numeric_limits<double>::max()));
        std::vector<double> values = UserInteractionAdapter::instance()->get_double_values(names, "LSMColumn "+objectName().toStdString(), "Parameters for 2D grid", limits);
        if(values.size() != 3) return;
        create_2d_grid(values[0],values[1],values[2]);
    }

    if("Create connections..." == action){
        std::vector<string> names;
        names.push_back("Distance with probability=0.5");
        std::vector<std::pair<double,double> > limits;
        limits.push_back(std::pair<double,double>(0,std::numeric_limits<double>::max()));
        std::vector<double> values = UserInteractionAdapter::instance()->get_double_values(names, "LSMColumn "+objectName().toStdString(), "Parameter for connection creation", limits);
        if(values.size() != 1) return;
        create_connections(values[0]);
    }

    if("Set synapse weights..." == action){
        std::vector<string> names;
        names.push_back("Mean");
        names.push_back("Variance");
        std::vector<std::pair<double,double> > limits;
        std::vector<double> values = UserInteractionAdapter::instance()->get_double_values(names, "LSMColumn "+objectName().toStdString(), "Setting synapse weights with normal distribution", limits);
        if(values.size() != 2) return;
        set_synapse_weights(values[0],values[1]);
    }

    if("Do Backpropagation" == action){
        do_backprop();
    }
}

std::set<SimulationObject*> Group::about_to_remove(SimulationObject* object){
    std::set<SimulationObject*> also_to_remove;
    if(this == object){
        BOOST_FOREACH(SimulationObject* o, m_objects){
            also_to_remove.insert(o);
        }
    }
    Samples* s = dynamic_cast<Samples*>(object);
    if(s){
        if(m_backprop_target == s) m_backprop_target = 0;
        m_inputs.erase(s);
    }

    return also_to_remove;
}

void Group::set_user_action_script(QString script){
    m_user_actions_script = script;
}

QString Group::user_action_script(){
    return m_user_actions_script;
}

void Group::set_update_script(QString script){
    m_update_script = script;
}

QString Group::update_script(){
    return m_update_script;
}

void Group::set_update_time_interval(double d){
    m_update_time_interval = d;
}

double Group::update_time_interval(){
    return m_update_time_interval;
}

void Group::set_drawn_horizontally(bool b){
    m_drawn_horizontally = b;
}

bool Group::drawn_horizontally() const{
    return m_drawn_horizontally;
}

void Group::set_backprop_target(Samples* s){
    m_backprop_target = s;
}

Samples* Group::backprop_target(){
    return m_backprop_target;
}

void Group::add_input(Samples* input){
    m_inputs.insert(input);
}

void Group::remove_input(Samples* input){
    m_inputs.erase(input);
}

std::set<Samples*> Group::inputs(){
    return m_inputs;
}

void Group::add_object(SimulationObject* object){
    m_objects.insert(object);
    simulation()->network()->add_object(object);
}

void Group::remove_object(SimulationObject* object){
    m_objects.erase(object);
}

std::set<SimulationObject*> Group::objects_as_std_set() const{
    return m_objects;
}

QObjectList Group::objects() const{
    QObjectList return_list;
    BOOST_FOREACH(SimulationObject* o, objects_as_std_set()){
        return_list.append(o);
    }
    return return_list;
}

SimulationObject* Group::object_by_name(QString name) const{
    foreach(QObject* object, objects()){
        if(object->objectName() == name)
            return dynamic_cast<SimulationObject*>(object);
    }
    return 0;
}

Point Group::handle_position() const{
    double min_z,max_z,min_y,max_y,min_x,max_x;
    min_z=max_z=min_y=max_y=min_x=max_x=0;
    Point pos;
    Point center = position();
    BOOST_FOREACH(SimulationObject* o, objects_as_std_set()){
        Point n_pos = o->position();
        pos = n_pos - center;
        if(pos.z < min_z) min_z = pos.z;
        if(pos.z > max_z) max_z = pos.z;
        if(pos.y < min_y) min_y = pos.y;
        if(pos.y > max_y) max_y = pos.y;
        if(pos.x < min_x) min_x = pos.x;
        if(pos.x > max_x) max_x = pos.x;
    }

    min_z -= MARGIN;
    min_y -= MARGIN;
    min_x -= MARGIN;
    max_z += MARGIN;
    max_y += MARGIN;
    max_x += MARGIN;

    if(drawn_horizontally())
        return Point(min_x,0,max_z);
    else
        return Point(0,min_y,max_z);
}

Point Group::moving_offset() const{
    return handle_position();
}

void Group::moved(Point new_position, Point old_position){
    Point vec = new_position - old_position;
    foreach(QObject* object, objects()){
        SimulationObject* o = dynamic_cast<SimulationObject*>(object);
        assert(o);
        o->set_position(o->position() + vec);
    }
}

std::set<Neuron*> Group::neurons(){
    return extract_all<Neuron>(m_objects);
}

std::set<LinearDiscriminator*> Group::linear_discriminators(){
    return extract_all<LinearDiscriminator>(m_objects);
}

void Group::set_mlp_output(LinearDiscriminator* ld){
    m_mlp_output = ld;
}

LinearDiscriminator* Group::mlp_output(){
    return m_mlp_output;
}

void Group::do_backprop(){
    LinearDiscriminator* output = m_mlp_output;
    Samples* target = m_backprop_target;
    assert(output);
    assert(target);


    std::set<LinearDiscriminator*> current_layer, next_layer;
    current_layer.insert(output);
    std::map<LinearDiscriminator*, double> errors;
    errors[output] = output->activation_function_derivative(output->membrane_potential()) * (target->value() - output->output());

    while(!current_layer.empty()){
        BOOST_FOREACH(LinearDiscriminator* ld, current_layer){
            BOOST_FOREACH(LDConnection* conn, ld->inputs()){
                next_layer.insert(conn->pre_neuron());
                errors[conn->pre_neuron()] += conn->weight() * errors[conn->post_neuron()];
            }
        }

        BOOST_FOREACH(LinearDiscriminator* ld, current_layer){
            BOOST_FOREACH(LDConnection* conn, ld->inputs()){
                errors[conn->pre_neuron()] *= conn->pre_neuron()->activation_function_derivative(conn->pre_neuron()->membrane_potential());
            }
        }
        current_layer = next_layer;
        next_layer.clear();
    }

    current_layer.insert(output);
    while(!current_layer.empty()){
        next_layer.clear();
        BOOST_FOREACH(LinearDiscriminator* ld, current_layer){
            BOOST_FOREACH(LDConnection* conn, ld->inputs()){
                double delta_w = m_backprop_eta * errors[conn->post_neuron()] * conn->pre_neuron()->output();
                conn->set_weight(conn->weight() + delta_w);
                next_layer.insert(conn->pre_neuron());
            }
        }
        current_layer = next_layer;
    }
}




double Group::backprop_eta(){
    return m_backprop_eta;
}

void Group::set_backprop_eta(double d){
    m_backprop_eta = d;
}
