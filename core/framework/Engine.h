//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_ENGINE_H
#define AETNA_ENGINE_H

#include "Script.h"
#include "render/Renderer.h"
#include "input/InputProxy.h"

#include <vector>
#include <string>
#include <chrono>

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

	std::string getExecutablePath();

	float getTime();
	float getFrameTime();
	int getFpsCount();

    bool isInitialized();
    Renderer *getRenderer();

    int getArgInt(int index);
    float getArgFloat(int index);
    const char *getArg(int index);
    int getNumArgs();

private:
    Engine();
    virtual ~Engine();

	std::chrono::time_point<std::chrono::system_clock> start_time;
	float frame_seconds;
	int fps_count;

    bool m_is_initialized = false;
    std::vector<Script*> m_scripts;

    enum class RendererType {
        OPENGL,
        DIRECT3D
    } renderer_type = RendererType::OPENGL;

    int window_width = 0;
    int window_height = 0;
    std::string window_title;

    int vsync_val = 0;

    Renderer *m_renderer;
    InputProxy* m_input_proxy;

    void update();
    void terminate();

    void process_args(int argc, char **argv);

    std::vector<const char*> app_args;
};


#endif //AETNA_ENGINE_H
