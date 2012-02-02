TEMPLATE = lib
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../NeuronModels ../math
CONFIG += staticlib

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




