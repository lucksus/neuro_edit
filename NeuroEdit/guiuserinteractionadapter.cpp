#include "guiuserinteractionadapter.h"
#include <QtGui/QFileDialog>

std::string GuiUserInteractionAdapter::get_save_filepath(std::string file_type, std::string source, std::string){
    return QFileDialog::getSaveFileName(0, source.c_str(), QString(), file_type.c_str()).toStdString();
}

std::string GuiUserInteractionAdapter::get_load_filepath(std::string file_type, std::string source, std::string porpuse){
    return QFileDialog::getOpenFileName(0, source.c_str(), QString(), file_type.c_str()).toStdString();
}

void GuiUserInteractionAdapter::display_samples(const std::vector<sample>& samples){

}

