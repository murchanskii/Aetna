#include "core/framework/Engine.h"
#include "MainScript.h"

int main(int argc, char *argv[])
{
    Engine *engine_ptr = Engine::get();
    engine_ptr->init(argc, argv);

    MainScript main_script;
    engine_ptr->addScript(&main_script);

    engine_ptr->run();

    return 0;
}