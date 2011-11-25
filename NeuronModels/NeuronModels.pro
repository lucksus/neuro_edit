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
INCLUDEPATH += ../math

win32{
	INCLUDEPATH += c:\boost\include
}else{
        INCLUDEPATH += /opt/local/include
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
    samples.cpp \
    photosssignalimporter.cpp \
    userinteractionadapter.cpp \
    lineardiscriminator.cpp \
    ldconnection.cpp \
    group.cpp \
    log.cpp

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
    samples.h \
    photosssignalimporter.h \
    userinteractionadapter.h \
    lineardiscriminator.h \
    ldconnection.h \
    group.h \
    qtstring_serialization.h \
    log.h
