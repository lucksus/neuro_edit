#include "multilayerperceptron.h"
#include <assert.h>

MultiLayerPerceptron::MultiLayerPerceptron(vector<unsigned int> number_of_units_per_layer)
    : m_number_of_units_per_layer(number_of_units_per_layer)
{
}


vector<double> MultiLayerPerceptron::forward_run(const vector<double> &input){
    assert(input.size() == m_number_of_units_per_layer[0]);

    m_membrane_potentials[0] = input;
    unsigned int layer = 0;
    while(layer < m_number_of_units_per_layer.size()){
        layer++;
        for(unsigned int i=0;i<m_number_of_units_per_layer[layer];i++){
            for(unsigned int j=0;j<m_number_of_units_per_layer[layer-1];j++){
                m_membrane_potentials[layer][i] += activation_function(m_membrane_potentials[layer-1][j]) * m_weights[layer-1][j][i];
            }
        }
    }

    return m_membrane_potentials[m_number_of_units_per_layer.size()-1];
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
        for(unsigned int i=0;i<m_number_of_units_per_layer[layer];i++)
            for(unsigned int j=0;j<m_number_of_units_per_layer[layer+1];j++)
                m_errors[layer][i] += m_errors[layer+1][j] * activation_function_derivative(m_membrane_potentials[layer+1][j]) * m_weights[layer][i][j];
    }
}

void MultiLayerPerceptron::update_weights(double eta){
    for(unsigned int layer = 0; layer < m_number_of_units_per_layer.size()-1; layer++)
        for(unsigned int i = 0; i < m_number_of_units_per_layer[layer]; i++)
            for(unsigned int j = 0; j< m_number_of_units_per_layer[layer+1]; j++)
                m_weights[layer][i][j] += m_errors[layer+1][j] * activation_function(m_membrane_potentials[layer][i]) * eta;
}


double MultiLayerPerceptron::activation_function(double x){
    return x;
}

double MultiLayerPerceptron::activation_function_derivative(double /*x*/){
    return 0;
}

void MultiLayerPerceptron::clear_errors_and_membranes(){
    for(unsigned int layer = 0; layer < m_number_of_units_per_layer.size(); layer++)
        for(unsigned int i = 0; i < m_number_of_units_per_layer[layer]; i++){
            m_errors[layer][i] = 0;
            m_membrane_potentials[layer][i] = 0;
        }
}
