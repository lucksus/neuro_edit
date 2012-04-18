#include "cmduserinteractionadapter.h"
#include <iostream>
#include <boost/foreach.hpp>
#include "lsmreadoutneuron.h"
#include <cstdio>
#include <assert.h>

using namespace std;

CmdUserInteractionAdapter::CmdUserInteractionAdapter()
{
}

std::string CmdUserInteractionAdapter::get_save_filepath(std::string file_type, std::string source, std::string purpose){
    cout << "--- Saving file ---" << endl
         << source << " wants to save a " << file_type << " file: " << purpose << endl
         << "Please provide file path: ";
    string path;
    cin >> path;
    cout << endl;
    return path;
}

std::string CmdUserInteractionAdapter::get_load_filepath(std::string file_type, std::string source, std::string purpose){
    cout << "--- Loading file ---" << endl
         << source << " wants to load a " << file_type << " file: " << purpose << endl
         << "Please provide file path: ";
    string path;
    cin >> path;
    cout << endl;
    return path;
}

void CmdUserInteractionAdapter::display_samples(const std::vector<sample>& samples){
    BOOST_FOREACH(const sample& s, samples){
        cout << s.time << "[ms]: " << s.value << endl;
    }
}

void CmdUserInteractionAdapter::display_image(QImage*){
    cout << "Can't display image in console mode!" << endl;
}

void CmdUserInteractionAdapter::display_mlp(MultiLayerPerceptron*){
    cout << "Can't display MLP in console mode!" << endl;
}

void CmdUserInteractionAdapter::display_read_out_weights(LSMReadOutNeuron* read_out){
    BOOST_FOREACH(const ReadOutConnection& c, read_out->read_out_connections()){
        cout << c.weight << "\t(" << c.liquid_unit->objectName().toStdString() << ")" << endl;
    }
}

vector<double> CmdUserInteractionAdapter::get_double_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<double,double> > limits ){
    assert(false);
    return vector<double>();
}

vector<int> CmdUserInteractionAdapter::get_integer_values(vector<string> value_descriptions, string source, string porpuse, vector<pair<int,int> > limits ){
    assert(false);
    return vector<int>();
}

void CmdUserInteractionAdapter::confirm_message(std::string message){
    cout << message << endl;
    cout << "(Press <Return> to continue...)";
    string bla;
    getline(cin, bla);
}
