#ifndef GUISETTINGS_H
#define GUISETTINGS_H

class GuiSettings
{
public:
    static GuiSettings& instance();

    void load();
    void save();

    struct Graphics{
        int neuron_detail, synapse_detail, axon_detail;
    }graphics;

    int hash();

private:
    GuiSettings();
};

#endif // GUISETTINGS_H
