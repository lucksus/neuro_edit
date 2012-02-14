#ifndef PHOTOSSSIGNALIMPORTER_H
#define PHOTOSSSIGNALIMPORTER_H
#include "samples.h"
#include <istream>
#include <map>
#include <string>
#include <vector>

class PhotossSignalImporter : public QObject
{
Q_OBJECT
public:
    PhotossSignalImporter(std::string filename);
    PhotossSignalImporter(std::istream* file);

    enum SampleType{ABSOLUTE_VALUES, PHASE_VALUES};


    std::vector<sample> read_samples(SampleType sample_type, unsigned int pol_plane);
    Q_INVOKABLE unsigned int number_of_bits();
    Q_INVOKABLE unsigned int samples_per_bit();
    Q_INVOKABLE double sampling_time();

    Q_INVOKABLE void set_time_stretch(double);
    Q_INVOKABLE void set_value_stretch(double);

    Q_INVOKABLE bool bit_at_time(double time_ps);

private:
    std::istream* m_file;
    std::map<std::string,std::string> m_simulation_parameters;
    std::vector<bool> m_bitpattern;

    double m_time_stretch;
    double m_value_stretch;

    void read_simulation_parameters();
    void read_bitpattern();

    void read_till(std::string);


    static const unsigned int SIZE = 1024;
    char c_line[SIZE];
};

#endif // PHOTOSSSIGNALIMPORTER_H
