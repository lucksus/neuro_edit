#include "mnistdata.h"
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QApplication>
#include <QGridLayout>
#include <QScrollArea>

int main(int argc, char** argv){
    QApplication app(argc,argv);
    QString images_file_name = QFileDialog::getOpenFileName(0, "Open Images File");
    QString labels_file_name = QFileDialog::getOpenFileName(0, "Open Labes File");
    MNISTData data;
    data.load_images(images_file_name);
    data.load_labels(labels_file_name);



    QWidget w;
    w.setLayout(new QGridLayout());


    unsigned int column_count = 20;
    unsigned int row = 0;
    unsigned int column = 0;
    for(unsigned int i=0;i<100;i++){
        QLabel *l = new QLabel;
        QImage* image = data.image_as_qimage(i);
        l->setPixmap(QPixmap::fromImage(*image));
        //delete image;
        qobject_cast<QGridLayout*>(w.layout())->addWidget(l,row,column);
        column++;
        if(column > column_count){
            column = 0;
            row++;
        }
    }



    QScrollArea scroll_area;
    scroll_area.setWidget(&w);
    scroll_area.show();

    app.exec();
}
