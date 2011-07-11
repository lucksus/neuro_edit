#ifndef GUIUSERINTERACTIONADAPTER_H
#define GUIUSERINTERACTIONADAPTER_H

#include "userinteractionadapter.h"

class GuiUserInteractionAdapter : public UserInteractionAdapter
{
public:
    virtual std::string get_save_filepath(std::string file_type, std::string source, std::string porpuse);
    virtual std::string get_load_filepath(std::string file_type, std::string source, std::string porpuse);
    virtual void display_samples(const std::vector<sample>& samples);
    virtual vector<double> get_double_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<double,double> > limits );
};

#endif // GUIUSERINTERACTIONADAPTER_H
