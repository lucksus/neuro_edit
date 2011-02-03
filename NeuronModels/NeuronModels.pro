#-------------------------------------------------
#
# Project created by QtCreator 2010-12-16T11:02:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = neuron_models
TEMPLATE = lib
CONFIG += staticlib

SOURCES += izhikevich.cpp \
           network.cpp \
    link.cpp \
    neuron.cpp \
    simulationobject.cpp \
    spatialobject.cpp \
    simulation.cpp

HEADERS += izhikevich.h \
           network.h \
    neuron.h \
    link.h \
    simulationobject.h \
    spatialobject.h \
    point.h \
    simulation.h
