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
    axon.cpp \
    neuron.cpp \
    simulationobject.cpp \
    spatialobject.cpp \
    simulation.cpp \
    current_inducer.cpp \
    synapse.cpp \
    spikenode.cpp \
    spikereceiver.cpp \
    spikeemitter.cpp \
    dendriticnode.cpp \
    neuronmodel.cpp \
    editableobject.cpp

HEADERS += izhikevich.h \
           network.h \
    neuron.h \
    axon.h \
    simulationobject.h \
    spatialobject.h \
    point.h \
    simulation.h \
    current_inducer.h \
    synapse.h \
    spikenode.h \
    spikereceiver.h \
    spikeemitter.h \
    dendriticnode.h \
    neuronmodel.h \
    editableobject.h
