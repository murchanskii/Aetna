//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_ENGINE_H
#define AETNA_ENGINE_H

#include "Script.h"
#include "render/Renderer.h"

#include <vector>
#include <string>

class Engine {
public:
    static Engine *get()
    {
        static Engine instance;
        return &instance;
    }

    Engine(Engine const&) = delete;
    Engine& operator=(Engine const&) = delete;

    void initialize(int argc, char **argv);
    void run();
    void addScript(Script *script);
    void removeScript(Script *script);

    bool isInitialized();
    Renderer *getRenderer();

private:
    Engine();
    virtual ~Engine();

    bool m_is_initialized = false;
    std::vector<Script*> m_scripts;

    enum class RendererType {
        OPENGL,
        DIRECT3D
    } renderer_type = RendererType::OPENGL;

    int window_width = 0;
    int window_height = 0;
    std::string window_title;

    Renderer *m_renderer;

    void update();
    void terminate();

    int process_args(int argc, char **argv);
};


#endif //AETNA_ENGINE_H
