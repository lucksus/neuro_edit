#-------------------------------------------------
#
# Project created by QtCreator 2011-01-03T16:14:12
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = NeuroEdit
TEMPLATE = app
INCLUDEPATH += ../NeuronModels
LIBS += -L../NeuronModels -lneuron_models

macx{
    LIBS += -framework GLUT
}

SOURCES += main.cpp\
        mainwindow.cpp \
    glscene.cpp \
    neuron_properties_widget.cpp \
    izhikevich_properties_widget.cpp \
    simulation_settings_widget.cpp

HEADERS  += mainwindow.h \
    glscene.h \
    neuron_properties_widget.h \
    izhikevich_properties_widget.h \
    simulation_settings_widget.h

FORMS    += mainwindow.ui \
    neuron_properties_widget.ui \
    izhikevich_properties_widget.ui \
    simulation_settings_widget.ui

RESOURCES += \
    icons.qrc
