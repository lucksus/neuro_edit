![logo](https://raw.github.com/lucksus/neuro_edit/master/NeuroEdit/Resources/splash.png)
neuro_edit
==========

neuro_edit is a neural net viewer, editor and simulator.
Nets can be build up by hand, copy&past and also
via scripts written in JavaScript (i.e. ECMA script).

It calculates a (3rd gen) [spiking neural net model](http://en.wikipedia.org/wiki/Spiking_neural_network).
This means, time and space are incorporated into the calculation.
Every single action potential is represented
and its propagation along the axon with finite velocity is simulated.

The [Izhikevich](http://www.izhikevich.org/human_brain_simulation/Blue_Brain.htm#models of spiking neurons")
neuron model is used to represent neurons.

![screenshot](https://raw.github.com/lucksus/neuro_edit/master/NeuroEdit/Resources/screenshot.png)

neuro_edit incorporates also a 2nd gen linear discriminator
neuron model. Combining a pool of Izhikevich neurons with a
linear discriminator read out neuron, neuro_edit
it is able to simulate [Liquid State Machines](http://www.igi.tugraz.at/psfiles/189.pdf).

Requirements
------------

To build neuro_edit, you need the following requirements:
* gcc or llvm/clang
* [boost](http://www.boost.org/) (>= 1.40)
* [Qt](http://qt-project.org/) (4.8.x)
* [QScintilla](http://www.riverbankcomputing.com/software/qscintilla/intro)

Build
-----
In order to build the whole project, change into the cloned directory and say

    qmake
    make
    
or open the .pro file with [Qt-Creator](http://qt-project.org/wiki/Category:Tools::QtCreator).
If you don't have installed boost and QScintilla to the system's
include and lib directories you'll need to give something like 

    INCLUDEPATH += /path/to/boost/inc /path/to/qscintilla/inc
    LIBS += /path/to/boost/lib /path/to/qscintilla/lib
    
to qmake.



