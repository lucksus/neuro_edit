TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../NeuronModels ../math
CONFIG += staticlib

# Input
HEADERS += \ 
    multilayerperceptron.h \
    mnistdata.h
SOURCES += \ 
    multilayerperceptron.cpp \
    mnistdata.cpp \
    test_mnist_data.cpp




