#-------------------------------------------------
#
# Project created by QtCreator 2010-12-16T11:02:46
#
#-------------------------------------------------

QT       -= core gui
CONFIG += debug
TARGET = neuron_models
TEMPLATE = lib
CONFIG += staticlib

win32{
	INCLUDEPATH += c:\boost\include
}

SOURCES += izhikevich.cpp \
           network.cpp \
    axon.cpp \
    neuron.cpp \
    simulationobject.cpp \
    simulation.cpp \
    current_inducer.cpp \
    synapse.cpp \
    dendriticnode.cpp \
    neuronmodel.cpp \
    serializationhelper.cpp \
    axonnode.cpp \
    scriptengine.cpp \
    spikingobject.cpp \
    controller.cpp \
    samples.cpp

HEADERS += izhikevich.h \
           network.h \
    neuron.h \
    axon.h \
    simulationobject.h \
    point.h \
    simulation.h \
    current_inducer.h \
    synapse.h \
    dendriticnode.h \
    neuronmodel.h \
    serializationhelper.h \
    axonnode.h \
    scriptengine.h \
    spikingobject.h \
    controller.h \
    samples.h
