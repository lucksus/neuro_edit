#ifndef MNISTDATA_H
#define MNISTDATA_H
#include <string>
#include <vector>
#include <QtGui/QImage>
#include <QtCore/QVector>

using namespace std;

class MNISTData : public QObject
{
Q_OBJECT
public:
    MNISTData();

    Q_INVOKABLE void load_images(QString path);
    Q_INVOKABLE void load_labels(QString path);

    Q_INVOKABLE unsigned int number_of_images();
    Q_INVOKABLE unsigned int number_of_rows();
    Q_INVOKABLE unsigned int number_of_columns();

    vector<unsigned char> image_raw_stl(unsigned int index);
    Q_INVOKABLE QVector<unsigned char> image_raw(unsigned int index);
    Q_INVOKABLE QImage* image_as_qimage(unsigned int index);
    Q_INVOKABLE unsigned char label(unsigned int index);

    Q_INVOKABLE void show_image(unsigned int index);


private:
    vector<unsigned char> m_labels;
    vector<  vector<unsigned char>  > m_images;
    unsigned int m_rows, m_columns;
};

#endif // MNISTDATA_H
