#ifndef CMDUSERINTERACTIONADAPTER_H
#define CMDUSERINTERACTIONADAPTER_H
#include "userinteractionadapter.h"

class CmdUserInteractionAdapter : public UserInteractionAdapter
{
public:
    CmdUserInteractionAdapter();

    virtual std::string get_save_filepath(std::string file_type, std::string source, std::string purpose);
    virtual std::string get_load_filepath(std::string file_type, std::string source, std::string purpose);
    virtual void display_samples(const std::vector<sample>& samples);
    virtual void display_image(QImage* image);
    virtual void display_mlp(MultiLayerPerceptron*);
    virtual void display_read_out_weights(LSMReadOutNeuron*);

    virtual vector<double> get_double_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<double,double> > limits );
    virtual vector<int> get_integer_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<int,int> > limits );

    virtual void confirm_message(std::string message);
};

#endif // CMDUSERINTERACTIONADAPTER_H
