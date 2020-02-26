//
// Created by murchanskii on 27.10.2019.
//

#include "Engine.h"
#include "Game.h"
#include "render/OpenGLRenderer.h"

#ifdef _WIN32
#include <Windows.h>
#include <comdef.h>
#endif

#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstring>

Engine::Engine() : start_time(std::chrono::system_clock::now()), m_renderer(nullptr), frame_seconds(0.0f), fps_count(0) {

}

Engine::~Engine() {

}

void Engine::initialize(int argc, char **argv) {
    if (isInitialized()) {
        return;
    }

    if (process_args(argc, argv) == -1) {
        return;
    }

    if (window_width == 0) {
        window_width = 800;
    }
    if (window_height == 0) {
        window_height = 600;
    }
    if (window_title.empty()) {
        window_title = "Aetna";
    }

    switch(renderer_type) {
        case RendererType::OPENGL:
            m_renderer = OpenGLRenderer::get();
            break;
        case RendererType::DIRECT3D:
            std::cerr << "Direct3D renderer is not currently implemented, use --renderer GL instead" << std::endl;
            return;
    }

    m_renderer->initialize(window_width, window_height, window_title.c_str());
    m_is_initialized = true;
}

void Engine::update() {
    for (Script *script : m_scripts) {
        script->update();
    }
    Game::get()->update();
    m_renderer->update();

    for (Script *script : m_scripts) {
        script->render();
    }
    Game::get()->render();
    m_renderer->render();
}

void Engine::terminate() {
    if (!isInitialized()) {
        return;
    }

    for (Script *script : m_scripts) {
        script->terminate();
    }

    m_renderer->terminate();
}

void Engine::run() {
    if (!isInitialized()) {
        return;
    }

	std::chrono::time_point<std::chrono::system_clock> frame_time;
	float last_time = getTime();

    while (m_renderer->mainWindowIsOpen()) {
		++fps_count;
		frame_time = std::chrono::system_clock::now();
        update();
        m_renderer->swap();
		frame_seconds = std::chrono::duration<float>(std::chrono::system_clock::now() - frame_time).count();

		if (getTime() - last_time >= 1.0f) {
			last_time = getTime();
			fps_count = 0;
		}
    }

    terminate();
}

bool Engine::isInitialized() {
    return m_is_initialized;
}

void Engine::addScript(Script *script) {
    if (!isInitialized()) {
        return;
    }

    m_scripts.push_back(script);
    script->initialize();
}

void Engine::removeScript(Script *script) {
    if (!isInitialized()) {
        return;
    }

    script->terminate();
    m_scripts.erase(std::find(m_scripts.begin(), m_scripts.end(), script));
}

std::string Engine::getExecutablePath()
{
#ifdef _WIN32
	WCHAR wpath[MAX_PATH];
	GetModuleFileNameW(NULL, wpath, MAX_PATH);
	_bstr_t conv_path(wpath);
	std::string path = conv_path;
	path = path.substr(0, path.rfind('\\'));
	return path;
#elif __linux__

#endif
	return nullptr;
}

float Engine::getTime()
{
	return std::chrono::duration<float>(std::chrono::system_clock::now() - start_time).count();
}

float Engine::getFrameTime()
{
	return frame_seconds;
}

int Engine::getFpsCount()
{
	return fps_count;
}

int Engine::process_args(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] <<
        " <-r | --renderer <option: GL / D3D> >" << std::endl;
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-r") == 0 ||
            strcmp(argv[i], "--renderer") == 0) {
            std::stringstream usage_message;
            usage_message << "usage: " << argv[i] << " <option: GL / D3D>";

            if (i + 1 == argc) {
                std::cerr << usage_message.str() << std::endl;
                return -1;
            }

            char *renderer_arg = argv[i + 1];
            if (strcmp(renderer_arg, "GL") == 0) {
                renderer_type = RendererType::OPENGL;
            }
            else if (strcmp(renderer_arg, "D3D") == 0) {
                renderer_type = RendererType::DIRECT3D;
                std::cerr << "Direct3D renderer is not currently implemented, use " <<
                argv[i] << " GL instead" << std::endl;
                return -1;
            }
            else {
                std::cerr << usage_message.str() << std::endl;
                return -1;
            }
            ++i;
        }
        else if (strcmp(argv[i], "-w_w") == 0 ||
                strcmp(argv[i], "--window_width") == 0 ) {
            std::stringstream usage_message;
            usage_message << "usage: " << argv[i] << " <option: <integer> >";

            if (i + 1 == argc) {
                std::cerr << usage_message.str() << std::endl;
                return -1;
            }

            int window_w = std::atoi(argv[i + 1]);
            if (window_w > 0) {
                window_width = window_w;
            }
            else {
                std::cerr << usage_message.str() << std::endl;
                return -1;
            }
            ++i;
        }
        else if (strcmp(argv[i], "-w_h") == 0 ||
                 strcmp(argv[i], "--window_height") == 0 ) {
            std::stringstream usage_message;
            usage_message << "usage: " << argv[i] << " <option: <integer> >";

            if (i + 1 == argc) {
                std::cerr << usage_message.str() << std::endl;
                return -1;
            }

            int window_h = std::atoi(argv[i + 1]);
            if (window_h > 0) {
                window_height = window_h;
            }
            else {
                std::cerr << usage_message.str() << std::endl;
                return -1;
            }
            ++i;
        }
        else if (strcmp(argv[i], "-w_t") == 0 ||
                 strcmp(argv[i], "--window_title") == 0 ) {
            std::stringstream usage_message;
            usage_message << "usage: " << argv[i] << " <option: <string> >";

            if (i + 1 == argc) {
                std::cerr << usage_message.str() << std::endl;
                return -1;
            }

            char *window_t = argv[i + 1];
            window_title = window_t;
            ++i;
        }
        else {
            std::cerr << "unknown argument: " << argv[i] << std::endl;
            return -1;
        }
    }
    return 1;
}

Renderer *Engine::getRenderer() {
    return m_renderer;
}
