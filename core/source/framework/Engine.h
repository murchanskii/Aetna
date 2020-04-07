#ifndef AETNA_ENGINE_H
#define AETNA_ENGINE_H

#include <framework/Application.h>
#include <framework/Game.h>

#include <framework/Script.h>

#include <vector>
#include <string>

class Engine {
public:
    enum class AppAPI {
        DETECT_MANUALLY = 0,
        OPENGL,
        DIRECT3D
    };

    static Engine *get()
    {
        static Engine instance;
        return &instance;
    }

    Engine(Engine const&) = delete;
    Engine& operator=(Engine const&) = delete;

    void initialize(int argc, char **argv, AppAPI api = AppAPI::DETECT_MANUALLY);
    void run();
    void terminate();

    const char* getCorePath();
    const char* getAssetsPath();

    void addScript(Script *script);
    void removeScript(Script *script);

    int getArgInt(int index);
    float getArgFloat(int index);
    const char* getArg(int index);
    int findArg(const char* name);
    int getNumArgs();

private:
    Engine();
    virtual ~Engine();

    std::string m_core_path;    // must be assigned through project file
    std::string m_assets_path;  // must be assigned through argv

    std::vector<Script*> m_scripts;
    std::vector<std::string> vec_argv;
    Application* m_app;
    Game* m_game;
    bool m_initialized;

    void process_args(int argc, char** argv);
    void detect_assets_path();

    void initialize_project();
    void initialize_materials();
    void initialize_application(AppAPI api);
    void initialize_game();

    void update();
};


#endif //AETNA_ENGINE_H
