#-------------------------------------------------
#
# Project created by QtCreator 2012-03-19T19:11:04
#
#-------------------------------------------------

QT       += core opengl script scripttools

QT       += gui

TARGET = NeuroRunner
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../NeuronModels ../math ../MLP
POST_TARGETDEPS += ../NeuronModels/libneuron_models.a ../math/libmath.a ../MLP/libMLP.a

LIBS += -L../NeuronModels -lneuron_models -L../math -lmath -L../MLP -lMLP

win32{
        INCLUDEPATH += c:\boost\include
        LIBS += -Lc:\boost\lib -llibboost_serialization-vc80-mt
}else{
        LIBS += -lboost_serialization
}

build_pass:CONFIG(debug, debug|release) {
        LIBS += -L../NeuronModels/debug
} else {
    LIBS += -L../NeuronModels/release
}

SOURCES += main.cpp \
    application.cpp \
    cmduserinteractionadapter.cpp

HEADERS += \
    application.h \
    cmduserinteractionadapter.h
