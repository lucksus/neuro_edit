#include "application.h"
#include "controller.h"
#include <iostream>
#include "scriptengine.h"
using namespace std;

Application::Application()
{
}

Application& Application::instance(){
    static Application a;
    return a;
}

void Application::init(){
    Controller::instance().init();
    Controller::instance().set_std_output(true);
}

void Application::destroy(){

}


void Application::do_script_console(){
    cout << "Greetings earthling. This is neuro_edit " << Controller::instance().version_string() << "." << endl;
    cout << "Entering interactive script console mode..." << endl;

    string line;
    do{
        cout << "# ";
        getline(cin,line);
        if(line == "exit") break;
        cout << Controller::instance().script_engine()->evaluate(line.c_str()).toStdString() << endl;
    }while(true);

    cout << "Exiting neuro_edit. Bye..." << endl;
}


int Application::return_value(){
    return 0;
}
