#include "mnistdata.h"
#include <fstream>
#include "userinteractionadapter.h"
#include "multilayerperceptron.h"
#include "log.h"

inline bool is_little_endian(){
    int x = 1;
    if(*(char *)&x == 1)
        return true;
    else
        return false;
}

inline void endian_swap(unsigned int& x)
{
    x = (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}




MNISTData::MNISTData()
{
}

void MNISTData::load_images(QString path){
    ifstream file;
    file.open(path.toStdString().c_str(), ios::binary);
    file.seekg(4, ios::beg); //skip magic number
    unsigned int number_of_images;
    file.read((char*)&number_of_images, 4);
    file.read((char*)&m_rows, 4);
    file.read((char*)&m_columns, 4);
    if(is_little_endian()){
        endian_swap(number_of_images);
        endian_swap(m_rows);
        endian_swap(m_columns);
    }

    m_images.resize(number_of_images);
    unsigned int pixels_in_image = m_rows * m_columns;
    for(unsigned int i=0;i<number_of_images;i++){
        m_images[i].resize(pixels_in_image);
        char c;
        for(unsigned int j=0;j<pixels_in_image;j++){
            file.read(&c,1);
            m_images[i][j] = c;
        }
    }
}

void MNISTData::load_labels(QString path){
    ifstream file;
    file.open(path.toStdString().c_str());
    file.seekg(4, ios::beg); //skip magic number
    unsigned int size;
    file.read((char*)&size, 4);
    if(is_little_endian()) endian_swap(size);

    m_labels.resize(size);
    char c;
    for(unsigned int i=0;i<size;i++){
        file.read(&c,1);
        m_labels[i] = c;
    }
}


unsigned int MNISTData::number_of_images(){
    return m_images.size();
}

unsigned int MNISTData::number_of_rows(){
    return m_rows;
}

unsigned int MNISTData::number_of_columns(){
    return m_columns;
}

vector<unsigned char> MNISTData::image_raw(unsigned int index){
    return m_images[index];
}

unsigned char MNISTData::label(unsigned int index){
    return static_cast<int>(m_labels[index]);
}

QImage* MNISTData::image_as_qimage(unsigned int index){
    QImage *qimage = new QImage(number_of_rows(), number_of_columns(), QImage::Format_RGB32);
    vector<unsigned char> image_vector = image_raw(index);
    for(unsigned int r=0;r<number_of_rows();r++)
        for(unsigned int c=0;c<number_of_columns();c++){
            char gray_value = 255 - image_vector[c+r*number_of_columns()];
            qimage->setPixel(c,r,qRgb(gray_value, gray_value, gray_value));
        }
    return qimage;
}

void MNISTData::show_image(unsigned int index){
    UserInteractionAdapter::instance()->display_image(image_as_qimage(index));
}

inline vector<double> to_vector_double(const vector<unsigned char> &in){
    vector<double> r;
    r.resize(in.size());
    for(unsigned int i=0;i<in.size();i++) r[i] = static_cast<double>(in[i]);
    return r;
}

void MNISTData::train(MultiLayerPerceptron* mlp, unsigned int image_count){
    if(image_count == 0) image_count = number_of_images();
    assert(mlp->units_in_layer(0) == m_rows*m_columns);
    assert(mlp->units_in_layer(mlp->layers()-1) == 10);
    vector<double> target;
    target.resize(10,0);
    for(unsigned int i = 0; i < image_count; i++){
        Log::instance().log(QString("Learning image %1 ...").arg(i).toStdString(), this);
        mlp->forward_run(to_vector_double(m_images[i]));
        target.resize(10,0);
        target[label(i)] = 1;
        mlp->backward_run(target);
        mlp->update_weights(0.2);
    }
}
