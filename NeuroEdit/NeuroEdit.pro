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
    simulation_settings_widget.cpp \
    neuron_membrane_plot_widget.cpp \
    glplotwidget2d.cpp \
    izhikevich_systemstate_plotwidget.cpp \
    application.cpp \
    neuron_manipulator_widget.cpp

HEADERS  += mainwindow.h \
    glscene.h \
    neuron_properties_widget.h \
    izhikevich_properties_widget.h \
    simulation_settings_widget.h \
    neuron_membrane_plot_widget.h \
    glplotwidget2d.h \
    izhikevich_systemstate_plotwidget.h \
    application.h \
    neuron_manipulator_widget.h

FORMS    += mainwindow.ui \
    neuron_properties_widget.ui \
    izhikevich_properties_widget.ui \
    simulation_settings_widget.ui \
    neuron_membrane_plot_widget.ui \
    neuron_manipulator_widget.ui

RESOURCES += \
    icons.qrc
