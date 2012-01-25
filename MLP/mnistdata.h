#ifndef MNISTDATA_H
#define MNISTDATA_H
#include <string>
#include <vector>
#include <QtGui/QImage>

using namespace std;

class MNISTData
{
public:
    MNISTData();

    void load_images(string path);
    void load_labels(string path);

    unsigned int number_of_images();
    unsigned int number_of_rows();
    unsigned int number_of_columns();

    vector<char> image(unsigned int index);
    QImage* image_as_qimage(unsigned int index);
    int label(unsigned int index);


private:
    vector<char> m_labels;
    vector<  vector<char>  > m_images;
    unsigned int m_rows, m_columns;
};

#endif // MNISTDATA_H
