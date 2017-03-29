#ifndef MNISTDATA_H
#define MNISTDATA_H
#include <string>
#include <vector>
#include <QImage>
#include <QVector>
#include "simulationobject.h"

using namespace std;

class MultiLayerPerceptron;
class MNISTData : public SimulationObject
{
Q_OBJECT
public:
    MNISTData();

    Q_INVOKABLE void load_images(QString path);
    Q_INVOKABLE void load_labels(QString path);

    Q_INVOKABLE unsigned int number_of_images();
    Q_INVOKABLE unsigned int number_of_rows();
    Q_INVOKABLE unsigned int number_of_columns();

    Q_INVOKABLE vector<unsigned char> image_raw(unsigned int index);
    Q_INVOKABLE QImage* image_as_qimage(unsigned int index);
    Q_INVOKABLE unsigned char label(unsigned int index);

    Q_INVOKABLE void show_image(unsigned int index);

    Q_INVOKABLE void train(MultiLayerPerceptron* mlp, unsigned int image_count = 0);

    virtual void update(double /*milli_seconds*/){}

private:
    vector<unsigned char> m_labels;
    vector<  vector<unsigned char>  > m_images;
    unsigned int m_rows, m_columns;
};


#endif // MNISTDATA_H
