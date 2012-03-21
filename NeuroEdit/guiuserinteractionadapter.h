#ifndef GUIUSERINTERACTIONADAPTER_H
#define GUIUSERINTERACTIONADAPTER_H

#include "userinteractionadapter.h"
#include "VisualizerContext2D.h"

class GuiUserInteractionAdapter : public UserInteractionAdapter
{
Q_OBJECT
public:
    GuiUserInteractionAdapter();
public slots:
    virtual std::string get_save_filepath(std::string file_type, std::string source, std::string porpuse);
    virtual std::string get_load_filepath(std::string file_type, std::string source, std::string porpuse);
    virtual void display_samples(const std::vector<sample>& samples, string name);
    virtual void display_image(QImage *image);
    virtual void display_mlp(MultiLayerPerceptron *);
    virtual void display_read_out_weights(LSMReadOutNeuron*);
    virtual vector<double> get_double_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<double,double> > limits );
    virtual vector<int> get_integer_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<int,int> > limits );
    virtual void confirm_message(std::string message);

private:
    VisualizerContext2D* m_context;
    map<string,VisualizerData2D*> m_visualizer_datas;
};

#endif // GUIUSERINTERACTIONADAPTER_H
