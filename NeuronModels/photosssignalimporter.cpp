#include "photosssignalimporter.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

PhotossSignalImporter::PhotossSignalImporter(std::istream* file)
    : m_file(file), m_time_stretch(1000*1000), m_value_stretch(1000)
{
    read_simulation_parameters();
    read_bitpattern();
}

void PhotossSignalImporter::read_simulation_parameters(){
    read_till("%Simulation Parameters");
    m_file->getline(c_line, SIZE);

    std::string line;
    m_file->getline(c_line, SIZE);
    line = c_line;
    while(!boost::starts_with(line, "%")){
        std::vector<std::string> pieces;
        boost::split(pieces, line, boost::algorithm::is_any_of("="));
        m_file->getline(c_line, SIZE);
        line = c_line;
        if(pieces.size() < 2) continue;
        std::string name = pieces[0];
        std::string value = pieces[1];
        boost::split(pieces, value, boost::algorithm::is_any_of(" %,"));
        value = pieces[0];
        m_simulation_parameters[name] = value;
    }
}

void PhotossSignalImporter::read_till(std::string breakpoint){
    std::string line;
    while(!boost::starts_with(line, breakpoint) && !m_file->eof()){
        m_file->getline(c_line,SIZE);
        line = c_line;
    }
}


void PhotossSignalImporter::read_bitpattern(){
    read_till("Bit Pattern Vector");
    std::string line;
    m_file->getline(c_line, SIZE);
    line = c_line;
    std::vector<std::string> pieces;
    boost::split(pieces, line, boost::algorithm::is_any_of("="));
    std::string size_str = pieces[1];
    boost::algorithm::trim(size_str);
    int size = boost::lexical_cast<int>(size_str);
    m_bitpattern.resize(size);
    for(int i=0; i<size; i++){
        m_file->getline(c_line, SIZE);
        line = c_line;
        boost::algorithm::trim(line);
        m_bitpattern[i] = (1 == boost::lexical_cast<int>(line));
    }
}

std::vector<sample> PhotossSignalImporter::read_samples(SampleType sample_type, unsigned int pol_plane){
    m_file->seekg(0, std::ios_base::beg);
    read_till("%Sampled Signal");
    read_till("Number of Rows");
    std::string line;
    line = c_line;
    std::vector<std::string> pieces;
    boost::split(pieces, line, boost::algorithm::is_any_of("="));
    boost::algorithm::trim(pieces[1]);
    unsigned int size = boost::lexical_cast<unsigned int>(pieces[1]);

    std::vector<sample> samples;
    samples.resize(size);
    read_till("% Sample Values");

    double time_fac = m_time_stretch / (1000*1000); //ps -> ms
    double time, abs, arg;
    for(unsigned int i=0; i<size; i++){
        m_file->getline(c_line, SIZE);
        line = c_line;
        boost::split(pieces, line, boost::algorithm::is_any_of("\t"));
        boost::algorithm::trim(pieces[0]);
        boost::algorithm::trim(pieces[1]);
        boost::algorithm::trim(pieces[2]);
        time = boost::lexical_cast<double>(pieces[0]);
        abs = boost::lexical_cast<double>(pieces[1]);
        arg = boost::lexical_cast<double>(pieces[2]);
        samples[i].time = time * time_fac;
        switch(sample_type){
        case ABSOLUTE_VALUES:
            samples[i].value = abs * m_value_stretch;
            break;
        case PHASE_VALUES:
            samples[i].value = arg * m_value_stretch;
            break;
        }
    }

    return samples;


}

unsigned int PhotossSignalImporter::number_of_bits(){
    std::map<std::string,std::string>::iterator it = m_simulation_parameters.find("numberOfBits");
    assert(it != m_simulation_parameters.end());
    return boost::lexical_cast<unsigned int>(it->second);
}

unsigned int PhotossSignalImporter::samples_per_bit(){
    std::map<std::string,std::string>::iterator it = m_simulation_parameters.find("samplesPerBit");
    assert(it != m_simulation_parameters.end());
    return boost::lexical_cast<unsigned int>(it->second);
}

double PhotossSignalImporter::sampling_time_ps(){
    std::map<std::string,std::string>::iterator it = m_simulation_parameters.find("samplingTime");
    assert(it != m_simulation_parameters.end());
    return boost::lexical_cast<double>(it->second);
}


void PhotossSignalImporter::set_time_stretch(double s){
    m_time_stretch = s;
}

void PhotossSignalImporter::set_value_stretch(double s){
    m_value_stretch = s;
}
