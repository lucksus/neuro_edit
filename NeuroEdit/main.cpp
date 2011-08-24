#include <QtGui/QApplication>
#include "application.h"
#ifdef __linux__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
/*
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>


void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, 2);
  exit(1);
}
*/

int main(int argc, char *argv[])
{

    //signal(SIGSEGV, handler);   // install our handler
    glutInit(&argc,argv);
    QApplication qa(argc, argv);
    qa.processEvents();
    Application::instance().init();
    Application::instance().show_main_window();
    int return_value = qa.exec();
    Application::instance().destroy();
    return return_value;
}
