#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include <vector>
#include <ext/hash_map>
#include <QtCore/QObject>
#include <QtCore/QVector>

using namespace std;
using namespace __gnu_cxx;

class MultiLayerPerceptron : public QObject
{
Q_OBJECT
public:
    MultiLayerPerceptron(vector<unsigned int> number_of_units_per_layer);

    Q_INVOKABLE vector<double> forward_run(const vector<double> &input);
    Q_INVOKABLE void backward_run(const vector<double> &target_values);
    Q_INVOKABLE void update_weights(double eta);

    Q_INVOKABLE unsigned int layers() const;
    Q_INVOKABLE unsigned int units_in_layer(unsigned int) const;

    double activation_function(double x);
    double activation_function_derivative(double x);

private:
    vector<unsigned int> m_number_of_units_per_layer;
    vector<   vector< vector<double> >   > m_weights;
    vector<   vector<     double     >   > m_errors;
    vector<   vector<     double     >   > m_membrane_potentials;
    //hash_map<pair<int,int>, double> m_weights;

    void clear_errors_and_membranes();
    void init_weights_with_random();


};

#endif // MULTILAYERPERCEPTRON_H
