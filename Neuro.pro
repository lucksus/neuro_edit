TEMPLATE = subdirs
CONFIG += ordered

# All the projects in your application are sub-projects of your solution
SUBDIRS = Visualizer \
            math \
            NeuronModels \
            NeuroEdit \
            MLP \
    NeuroRunner



# Use .depends to specify that a project depends on another.
NeuroEdit.depends = NeuronModels Visualizer math MLP
NeuroRunner.depends = NeuronModels math MLP
