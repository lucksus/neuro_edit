#include "userinteractionadapter.h"

UserInteractionAdapter* UserInteractionAdapter::s_instance = 0;

void UserInteractionAdapter::install_instance(UserInteractionAdapter* instance){
    s_instance = instance;
}

UserInteractionAdapter* UserInteractionAdapter::instance(){
    return s_instance;
}
