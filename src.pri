CONFIG(debug, debug|release) {
     mac: TARGET = $$join(TARGET,,,_debug) 
     !mac: TARGET = $$join(TARGET,,,d)
}