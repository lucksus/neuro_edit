#-------------------------------------------------
#
# Project created by QtCreator 2011-01-03T16:14:12
#
#-------------------------------------------------

QT       += core gui opengl script scripttools
CONFIG += debug
TARGET = neuro_edit
TEMPLATE = app
INCLUDEPATH += ../NeuronModels ../Visualizer ../math ../MLP
POST_TARGETDEPS += ../NeuronModels/libneuron_models.a ../Visualizer/libVisualizer.a ../math/libmath.a ../MLP/libMLP.a

macx:LIBS += -framework GLUT

win32{
	INCLUDEPATH += c:\boost\include
	LIBS += -Lc:\boost\lib -llibboost_serialization-vc80-mt
}else{
        LIBS += -L/opt/local/lib  -lboost_serialization -lglut
        INCLUDEPATH += /opt/local/include
}

build_pass:CONFIG(debug, debug|release) {
        LIBS += -L../NeuronModels/debug -L../Visualizer/debug
} else {
    LIBS += -L../NeuronModels/release -L../Visualizer/debug
}

LIBS += -L../NeuronModels -lneuron_models -L../Visualizer -lVisualizer -L../math -lmath -L../MLP -lMLP

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
    guisettings.cpp

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
    guisettings.h

FORMS    += mainwindow.ui \
    izhikevich_properties_widget.ui \
    simulation_settings_widget.ui \
    neuron_membrane_plot_widget.ui \
    neuron_manipulator_widget.ui \
    about_dialog.ui \
    scriptswindow.ui \
    logwindow.ui \
    settingsdialog.ui

RESOURCES += \
    icons.qrc
ICON = Resources/logo.icns

include(qtpropertybrowser/qtpropertybrowser.pri)
