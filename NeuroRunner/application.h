#ifndef APPLICATION_H
#define APPLICATION_H

class Application
{
public:
    static Application& instance();
    void init();
    void destroy();
    void do_script_console();
    int return_value();

private:
    Application();
};

#endif // APPLICATION_H
