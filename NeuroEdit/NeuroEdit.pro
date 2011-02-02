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
    glscene.cpp

HEADERS  += mainwindow.h \
    glscene.h

FORMS    += mainwindow.ui
