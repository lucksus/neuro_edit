#-------------------------------------------------
#
# Project created by QtCreator 2012-03-19T19:11:04
#
#-------------------------------------------------

QT     += core opengl script scripttools
TARGET  = NeuroRunner
CONFIG += console
CONFIG -= app_bundle
CONFIG += debug_and_release
TEMPLATE = app
INCLUDEPATH += ../NeuronModels ../math ../MLP
DESTDIR=../bin
LIBS += -L../lib
include(../src.pri)


build_pass:CONFIG(debug, debug|release) {
    mac: LIBS +=  -lneuron_models_debug -lmath_debug -lMLP_debug
    !mac: LIBS +=  -lneuron_modelsd -lmathd -lMLPd
    mac: POST_TARGETDEPS += ../lib/libneuron_models_debug.a ../lib/libmath_debug.a ../lib/libMLP_debug.a
    unix: POST_TARGETDEPS += ../lib/libneuron_modelsd.a ../lib/libmathd.a ../lib/libMLPd.a
    win32: POST_TARGETDEPS += ../lib/neuron_modelsd.lib ../lib/mathd.lib ../lib/MLPd.lib
    unix: LIBS += -lboost_serialization
} else {
    LIBS += -lneuron_models -lmath -lMLP
    !win32: POST_TARGETDEPS += ../lib/libneuron_models.a ../lib/libmath.a ../lib/libMLP.a
    win32: POST_TARGETDEPS += ../lib/neuron_models.lib ../lib/math.lib ../lib/MLP.lib
    unix: LIBS += -lboost_serialization
}

#build_pass:CONFIG(debug, debug|release) {
#        LIBS += -L../NeuronModels/debug
#} else {
#    LIBS += -L../NeuronModels/release
#}

SOURCES += main.cpp \
    application.cpp \
    cmduserinteractionadapter.cpp

HEADERS += \
    application.h \
    cmduserinteractionadapter.h
