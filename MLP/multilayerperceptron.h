#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include <vector>
#include <ext/hash_map>

using namespace std;
using namespace __gnu_cxx;

class MultiLayerPerceptron
{
public:
    MultiLayerPerceptron(vector<unsigned int> number_of_units_per_layer);
    vector<double> forward_run(const vector<double> &input);
    void backward_run(const vector<double> &target_values);
    void update_weights(double eta);

    unsigned int layers() const;
    unsigned int units_in_layer(unsigned int) const;

    double activation_function(double x);
    double activation_function_derivative(double x);

private:
    vector<unsigned int> m_number_of_units_per_layer;
    vector<   vector< vector<double> >   > m_weights;
    vector<   vector<     double     >   > m_errors;
    vector<   vector<     double     >   > m_membrane_potentials;
    //hash_map<pair<int,int>, double> m_weights;

    void clear_errors_and_membranes();


};

#endif // MULTILAYERPERCEPTRON_H
