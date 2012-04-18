TEMPLATE = lib
DEPENDPATH += .
INCLUDEPATH += . ../NeuronModels ../math
CONFIG += staticlib
CONFIG += debug_and_release
DESTDIR=../lib
include(../src.pri)

# Input
HEADERS += \ 
    multilayerperceptron.h \
    mnistdata.h \
    mlpvisualization.h
SOURCES += \ 
    multilayerperceptron.cpp \
    mnistdata.cpp \
    test_mnist_data.cpp \
    mlpvisualization.cpp

FORMS += \
    mlpvisualization.ui




