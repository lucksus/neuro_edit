TEMPLATE = subdirs
CONFIG += ordered

# All the projects in your application are sub-projects of your solution
SUBDIRS = Visualizer \
            math \
            NeuronModels \
            NeuroEdit



# Use .depends to specify that a project depends on another.
NeuroEdit.depends = NeuronModels Visualizer math
