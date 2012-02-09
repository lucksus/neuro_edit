#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include <vector>
#include <QtCore/QObject>
#include <QtCore/QVector>
#include "simulationobject.h"
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

class MultiLayerPerceptron : public SimulationObject
{
Q_OBJECT
friend class MLPVisualization;
friend class boost::serialization::access;
public:
    MultiLayerPerceptron(vector<unsigned int> number_of_units_per_layer);

    Q_INVOKABLE vector<double> forward_run(const vector<double> &input);
    Q_INVOKABLE void backward_run(const vector<double> &target_values);
    Q_INVOKABLE void update_weights(double eta);

    Q_INVOKABLE void add_errors_to_average_update_vector(double eta);
    Q_INVOKABLE void apply_average_update_vector();
    Q_INVOKABLE void reset_average_update_vector();
    Q_INVOKABLE QString print_average_update_vector();

    Q_INVOKABLE unsigned int layers() const;
    Q_INVOKABLE unsigned int units_in_layer(unsigned int) const;

    double activation_function(double x);
    double activation_function_derivative(double x);

    virtual void update(double /*milli_seconds*/){};

    Q_INVOKABLE void save(QString path);
    static MultiLayerPerceptron* load(QString path);
    Q_INVOKABLE virtual QString print();

signals:
    void changed();

private:
    explicit MultiLayerPerceptron() {}
    vector<unsigned int> m_number_of_units_per_layer;
    vector<   vector< vector<double> >   > m_weights;
    vector<   vector<     double     >   > m_errors;
    vector<   vector<     double     >   > m_membrane_potentials;
    //hash_map<pair<int,int>, double> m_weights;

    vector<   vector< vector<double> >   > m_average_update_vector;
    unsigned int m_vectors_in_average_so_far;

    void clear_errors_and_membranes();
    void init_weights_with_random();
    void size_up_average_update_vector();


    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(m_number_of_units_per_layer);
        ar & BOOST_SERIALIZATION_NVP(m_weights);
        ar & BOOST_SERIALIZATION_NVP(m_errors);
        ar & BOOST_SERIALIZATION_NVP(m_membrane_potentials);
        size_up_average_update_vector();
    }


};


#endif // MULTILAYERPERCEPTRON_H
