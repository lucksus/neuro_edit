#-------------------------------------------------
#
# Project created by QtCreator 2011-01-03T16:14:12
#
#-------------------------------------------------

QT       += core gui opengl script scripttools
CONFIG += debug_and_release
TARGET = neuro_edit
TEMPLATE = app
INCLUDEPATH += ../NeuronModels ../Visualizer ../math ../MLP
DESTDIR=../bin
LIBS += -L../lib
include(../src.pri)

build_pass:CONFIG(debug, debug|release) {
    mac: LIBS +=  -lneuron_models_debug -lVisualizer_debug -lmath_debug -lMLP_debug
    !mac: LIBS +=  -lneuron_modelsd -lVisualizerd -lmathd -lMLPd
    mac: POST_TARGETDEPS += ../lib/libneuron_models_debug.a ../lib/libVisualizer_debug.a ../lib/libmath_debug.a ../lib/libMLP_debug.a
    unix: POST_TARGETDEPS += ../lib/libneuron_modelsd.a ../lib/libVisualizerd.a ../lib/libmathd.a ../lib/libMLPd.a
    win32: POST_TARGETDEPS += ../lib/neuron_modelsd.lib ../lib/Visualizerd.lib ../lib/mathd.lib ../lib/MLPd.lib
} else {
    LIBS +=  -lneuron_models -lVisualizer -lmath -lMLP
    !win32: POST_TARGETDEPS += ../lib/libneuron_models.a ../lib/libVisualizer.a ../lib/libmath.a ../lib/libMLP.a
    win32: POST_TARGETDEPS += ../lib/neuron_models.lib ../lib/Visualizer.lib ../lib/math.lib ../lib/MLP.lib
}

macx:LIBS += -framework GLUT

LIBS += -lqscintilla2
#LIBS += -lboost_serialization -lglut

SOURCES += main.cpp\
        mainwindow.cpp \
    glscene.cpp \
    izhikevich_properties_widget.cpp \
    simulation_settings_widget.cpp \
    neuron_membrane_plot_widget.cpp \
    glplotwidget2d.cpp \
    izhikevich_systemstate_plotwidget.cpp \
    application.cpp \
    about_dialog.cpp \
    drawable.cpp \
    drawableneuron.cpp \
    drawableaxon.cpp \
    drawableaxonnode.cpp \
    drawabledendritenode.cpp \
    glhelpfunctions.cpp \
    drawablesynapse.cpp \
    propertybrowser.cpp \
    scriptswindow.cpp \
    scriptsyntaxhighlighter.cpp \
    drawablecurrentinducer.cpp \
    drawablesamples.cpp \
    guiuserinteractionadapter.cpp \
    menuobjectrightclick.cpp \
    drawablelineardiscriminator.cpp \
    drawableldconnection.cpp \
    drawablegroup.cpp \
    logwindow.cpp \
    settingsdialog.cpp \
    guisettings.cpp \
    readoutvisualization.cpp

HEADERS  += mainwindow.h \
    glscene.h \
    izhikevich_properties_widget.h \
    simulation_settings_widget.h \
    neuron_membrane_plot_widget.h \
    glplotwidget2d.h \
    izhikevich_systemstate_plotwidget.h \
    application.h \
    about_dialog.h \
    drawable.h \
    drawableneuron.h \
    drawableaxon.h \
    drawableaxonnode.h \
    drawabledendritenode.h \
    glhelpfunctions.h \
    drawablesynapse.h \
    propertybrowser.h \
    scriptswindow.h \
    scriptsyntaxhighlighter.h \
    drawablecurrentinducer.h \
    drawablesamples.h \
    guiuserinteractionadapter.h \
    menuobjectrightclick.h \
    drawablelineardiscriminator.h \
    drawableldconnection.h \
    drawablegroup.h \
    logwindow.h \
    settingsdialog.h \
    guisettings.h \
    readoutvisualization.h

FORMS    += mainwindow.ui \
    izhikevich_properties_widget.ui \
    simulation_settings_widget.ui \
    neuron_membrane_plot_widget.ui \
    neuron_manipulator_widget.ui \
    about_dialog.ui \
    scriptswindow.ui \
    logwindow.ui \
    settingsdialog.ui \
    readoutvisualization.ui

RESOURCES += \
    icons.qrc
ICON = Resources/logo.icns

include(qtpropertybrowser/qtpropertybrowser.pri)
