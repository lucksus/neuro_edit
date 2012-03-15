#ifndef USERINTERACTIONADAPTER_H
#define USERINTERACTIONADAPTER_H

#include <string>
#include <vector>
#include "samples.h"
#include <QtGui/QImage>

using namespace std;

class MultiLayerPerceptron;
class LSMReadOutNeuron;

class UserInteractionAdapter : public QObject
{
public:
    static void install_instance(UserInteractionAdapter* instance);
    static UserInteractionAdapter* instance();

    virtual std::string get_save_filepath(std::string file_type, std::string source, std::string porpuse) = 0;
    virtual std::string get_load_filepath(std::string file_type, std::string source, std::string porpuse) = 0;
    virtual void display_samples(const std::vector<sample>& samples) = 0;
    virtual void display_image(QImage* image) = 0;
    virtual void display_mlp(MultiLayerPerceptron*) = 0;
    virtual void display_read_out_weights(LSMReadOutNeuron*) = 0;

    virtual vector<double> get_double_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<double,double> > limits ) = 0;
    virtual vector<int> get_integer_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<int,int> > limits ) = 0;

    virtual void confirm_message(std::string message) = 0;


private:
    static UserInteractionAdapter* s_instance;
};

#endif // USERINTERACTIONADAPTER_H
