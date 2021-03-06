#include "multilayerperceptron.h"
#include <assert.h>
#include <boost/foreach.hpp>
#include "RandomGenerator.h"
#include "math_constants.h"
#include <math.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>

using namespace NeuroMath;

MultiLayerPerceptron::MultiLayerPerceptron(vector<unsigned int> number_of_units_per_layer)
    : m_number_of_units_per_layer(number_of_units_per_layer)
{
    m_errors.resize(number_of_units_per_layer.size());
    m_membrane_potentials.resize(number_of_units_per_layer.size());
    m_weights.resize(number_of_units_per_layer.size()-1);
    m_average_update_vector.resize(number_of_units_per_layer.size()-1);

    for(unsigned int i=0;i<number_of_units_per_layer.size();i++){
        m_errors[i].resize(number_of_units_per_layer[i],0);
        m_membrane_potentials[i].resize(number_of_units_per_layer[i],0);
        if(i==number_of_units_per_layer.size()-1) continue;
        m_weights[i].resize(number_of_units_per_layer[i]);
        m_average_update_vector[i].resize(number_of_units_per_layer[i]);
        for(unsigned int j=0;j<number_of_units_per_layer[i];j++){
            m_weights[i][j].resize(number_of_units_per_layer[i+1],0);
            m_average_update_vector[i][j].resize(number_of_units_per_layer[i+1],0);
        }
    }

    init_weights_with_random();
}

void MultiLayerPerceptron::size_up_average_update_vector(){
    m_average_update_vector.resize(m_number_of_units_per_layer.size()-1);
    for(unsigned int i=0;i<m_number_of_units_per_layer.size();i++){
        m_average_update_vector[i].resize(m_number_of_units_per_layer[i]);
        for(unsigned int j=0;j<m_number_of_units_per_layer[i];j++){
            m_average_update_vector[i][j].resize(m_number_of_units_per_layer[i+1],0);
        }
    }
}

void MultiLayerPerceptron::init_weights_with_random(){
    for(unsigned int layer=0;layer<m_weights.size();layer++)
        for(unsigned int pre=0;pre<m_weights[layer].size();pre++)
            for(unsigned int post=0;post<m_weights[layer][pre].size();post++)
                m_weights[layer][pre][post] = RandomGenerator::getInstance()->uniform(-1,1);
}

vector<double> MultiLayerPerceptron::forward_run(const vector<double> &input){
    assert(input.size() == m_number_of_units_per_layer[0]);
    //clear_errors_and_membranes();
    m_membrane_potentials[0] = input;
    unsigned int layer = 0;
    while(layer < m_number_of_units_per_layer.size()-1){
        layer++;
        for(unsigned int i=0;i<m_number_of_units_per_layer[layer];i++){
            m_membrane_potentials[layer][i] = 0;
            for(unsigned int j=0;j<m_number_of_units_per_layer[layer-1];j++){
                double net = m_membrane_potentials[layer-1][j];
                double out = activation_function(net);
                double weight = m_weights[layer-1][j][i];
                m_membrane_potentials[layer][i] +=  out * weight;
            }
        }
    }

    vector<double> result = m_membrane_potentials[m_number_of_units_per_layer.size()-1];
    BOOST_FOREACH(double &d, result)
        d = activation_function(d);
    emit changed();
    return result;
}

void MultiLayerPerceptron::backward_run(const vector<double> &target_values){
    assert(target_values.size() == m_number_of_units_per_layer.back());

    int layer = m_number_of_units_per_layer.size()-1;
    vector<double> &last_layer = m_errors[layer];
    for(unsigned int i = 0; i<last_layer.size(); i++){
        last_layer[i] = (target_values[i] - activation_function(m_membrane_potentials[layer][i])) * activation_function_derivative(m_membrane_potentials[layer][i]);
    }

    layer--;
    for(; layer >= 0; layer--){
        for(unsigned int i=0;i<m_number_of_units_per_layer[layer];i++){
          m_errors[layer][i] = 0;
          for(unsigned int j=0;j<m_number_of_units_per_layer[layer+1];j++)
            m_errors[layer][i] += m_errors[layer+1][j] *  m_weights[layer][i][j];
          m_errors[layer][i] *= activation_function_derivative(m_membrane_potentials[layer][i]);
        }
    }
    emit changed();
}

void MultiLayerPerceptron::update_weights(double eta){
    for(unsigned int layer = 0; layer < m_number_of_units_per_layer.size()-1; layer++)
        for(unsigned int i = 0; i < m_number_of_units_per_layer[layer]; i++)
            for(unsigned int j = 0; j< m_number_of_units_per_layer[layer+1]; j++)
                m_weights[layer][i][j] += m_errors[layer+1][j] * activation_function(m_membrane_potentials[layer][i]) * eta;
    emit changed();
}

void MultiLayerPerceptron::add_errors_to_average_update_vector(double eta){

    for(unsigned int layer = 0; layer < m_number_of_units_per_layer.size()-1; layer++)
        for(unsigned int i = 0; i < m_number_of_units_per_layer[layer]; i++)
            for(unsigned int j = 0; j< m_number_of_units_per_layer[layer+1]; j++)
                if(m_vectors_in_average_so_far == 0)
                    m_average_update_vector[layer][i][j] = m_errors[layer+1][j] * activation_function(m_membrane_potentials[layer][i]) * eta;
                else
                    m_average_update_vector[layer][i][j] = m_vectors_in_average_so_far/(m_vectors_in_average_so_far+1) * m_average_update_vector[layer][i][j] +
                            1/(m_vectors_in_average_so_far+1) * m_errors[layer+1][j] * activation_function(m_membrane_potentials[layer][i]) * eta;

    m_vectors_in_average_so_far++;
}

void MultiLayerPerceptron::apply_average_update_vector(){
    for(unsigned int layer = 0; layer < m_number_of_units_per_layer.size()-1; layer++)
        for(unsigned int i = 0; i < m_number_of_units_per_layer[layer]; i++)
            for(unsigned int j = 0; j< m_number_of_units_per_layer[layer+1]; j++)
                m_weights[layer][i][j] += m_average_update_vector[layer][i][j];
    emit changed();
}

void MultiLayerPerceptron::reset_average_update_vector(){
    for(unsigned int layer = 0; layer < m_number_of_units_per_layer.size()-1; layer++)
        for(unsigned int i = 0; i < m_number_of_units_per_layer[layer]; i++)
            for(unsigned int j = 0; j< m_number_of_units_per_layer[layer+1]; j++)
                m_average_update_vector[layer][i][j] = 0;
    m_vectors_in_average_so_far = 0;
}

QString MultiLayerPerceptron::print_average_update_vector(){
    QString s;
    for(unsigned int layer=0; layer < m_number_of_units_per_layer.size()-1; layer++){
        s.append("Layer %1:\n").arg(layer+1);
        for(unsigned int pre=0; pre < m_number_of_units_per_layer[layer]; pre++){
            for(unsigned int post=0; post < m_number_of_units_per_layer[layer+1]; post++){
                s.append(QString::number(m_average_update_vector[layer][pre][post], 'f', 2));
                if(post < m_number_of_units_per_layer[layer+1]-1) s.append("\t");
                else s.append("\n");
            }
        }
    }
    return s;
}


double MultiLayerPerceptron::activation_function(double x){
    return 1/(1+pow(NeuroMath::e(),-x));
    //return x;
}

double MultiLayerPerceptron::activation_function_derivative(double x){
    return activation_function(x) * (1-x);
    //return 1;
}

void MultiLayerPerceptron::clear_errors_and_membranes(){
    for(unsigned int layer = 0; layer < m_number_of_units_per_layer.size(); layer++)
        for(unsigned int i = 0; i < m_number_of_units_per_layer[layer]; i++){
            m_errors[layer][i] = 0;
            m_membrane_potentials[layer][i] = 0;
        }
}

unsigned int MultiLayerPerceptron::layers() const{
    return m_number_of_units_per_layer.size();
}

unsigned int MultiLayerPerceptron::units_in_layer(unsigned int i) const{
    return m_number_of_units_per_layer[i];
}

void MultiLayerPerceptron::save(QString path){
    std::ofstream file;
    file.open(path.toStdString().c_str());
    boost::archive::binary_oarchive archive(file);
    archive.register_type<MultiLayerPerceptron>();
    archive << *this;
}

MultiLayerPerceptron* MultiLayerPerceptron::load(QString path){
    std::ifstream file;
    file.open(path.toStdString().c_str());
    boost::archive::binary_iarchive archive(file);
    archive.register_type<MultiLayerPerceptron>();
    MultiLayerPerceptron* mlp = new MultiLayerPerceptron();
    archive >> *mlp;
    return mlp;
}

QString MultiLayerPerceptron::print(){
    QString s;
    for(unsigned int layer=0; layer < m_number_of_units_per_layer.size()-1; layer++){
        s.append("Layer %1:\n").arg(layer+1);
        for(unsigned int pre=0; pre < m_number_of_units_per_layer[layer]; pre++){
            for(unsigned int post=0; post < m_number_of_units_per_layer[layer+1]; post++){
                s.append(QString::number(m_weights[layer][pre][post], 'f', 2));
                if(post < m_number_of_units_per_layer[layer+1]-1) s.append("\t");
                else s.append("\n");
            }
        }
    }
    return s;
}
