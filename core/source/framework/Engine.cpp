#include "Engine.h"

#include <framework/ApplicationGL.h>
#include <framework/Materials.h>

#include <algorithm>

Engine::Engine() : m_initialized(false) {

}

Engine::~Engine() {
    terminate();
}

void Engine::initialize(int argc, char** argv, AppAPI api) {
    process_args(argc, argv);
    initialize_project();

    if (m_core_path.empty() || m_assets_path.empty()) {
        const char* folder_names[] = {
            "Core",
            "Assets"
        };
        int folder_index = m_core_path.empty() ? 0 : m_assets_path.empty() ? 1 : 1;
        std::cerr << folder_names[folder_index] << " folder not specified" << std::endl;
        return;
    }
    
    initialize_materials();
    initialize_application(api);
    if (!m_app) {
        std::cerr << "Could not initialize application" << std::endl;
        return;
    }
    initialize_game();

    m_initialized = true;
}

void Engine::run() {
    if (m_initialized) {
        while (m_app->windowIsOpen()) {
            update();
        }
        terminate();
    }
}

void Engine::terminate() {
    if (!m_initialized) {
        return;
    }

    for (Script* script : m_scripts) {
        script->terminate();
    }

    Game::get()->terminate();

    if (m_app) {
        m_app->terminate();

        ApplicationGL* m_app_gl = dynamic_cast<ApplicationGL*>(m_app);
        if (m_app_gl) {
            delete m_app_gl;
            m_app = nullptr;
        }
    }

    m_initialized = false;
}

const char* Engine::getCorePath() {
    return m_core_path.c_str();
}

const char* Engine::getAssetsPath() {
    return m_assets_path.c_str();
}

void Engine::addScript(Script* script) {
    if (!m_initialized) {
        return;
    }

    m_scripts.push_back(script);
    script->initialize();
}

void Engine::removeScript(Script* script) {
    if (!m_initialized) {
        return;
    }

    script->terminate();
    m_scripts.erase(std::find(m_scripts.begin(), m_scripts.end(), script));
}

int Engine::getArgInt(int index) {
    return std::stoi(vec_argv[index]);
}

float Engine::getArgFloat(int index) {
    return std::stof(vec_argv[index]);
}

const char* Engine::getArg(int index) {
    return vec_argv[index].c_str();
}

int Engine::findArg(const char* name) {
    std::vector<std::string>::iterator it = std::find(vec_argv.begin(), vec_argv.end(), name);
    if (it != vec_argv.end()) {
        return it - vec_argv.begin();
    }
    return -1;
}

int Engine::getNumArgs() {
    return vec_argv.size();
}

void Engine::process_args(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        vec_argv.push_back(argv[i]);
    }
}

void Engine::detect_assets_path() {
    int assets_path_arg_i = findArg("-assets");
    if (assets_path_arg_i < 0 || assets_path_arg_i + 1 == getNumArgs()) {
        return;
    }
    m_assets_path = getArg(assets_path_arg_i + 1);
    if (m_assets_path.back() != '\\' && m_assets_path.back() != '/') {
        m_assets_path = m_assets_path + "/";
    }
}

void Engine::initialize_project() {
    m_core_path = ENGINE_PATH;
    if (m_core_path.back() != '\\' && m_core_path.back() != '/') {
        m_core_path = m_core_path + "/";
    }
    detect_assets_path();
}

void Engine::initialize_materials() {
    std::string path_to_materials_core = std::string(getCorePath()) + "materials";
    Materials::get()->initialize(path_to_materials_core.c_str());

    std::string path_to_materials_assets = getAssetsPath();
    Materials::get()->initialize(path_to_materials_assets.c_str());
}

void Engine::initialize_application(AppAPI api) {
    int app_width = 800;
    int app_height = 600;
    std::string app_title = AETNA_PROJECT_NAME;

    int window_w_arg_i = findArg("-window_w");
    if (window_w_arg_i >= 0 && window_w_arg_i + 1 < getNumArgs()) {
        app_width = getArgInt(window_w_arg_i + 1);
    }

    int window_h_arg_i = findArg("-window_h");
    if (window_h_arg_i >= 0 && window_h_arg_i + 1 < getNumArgs()) {
        app_height = getArgInt(window_h_arg_i + 1);
    }

    int window_title_arg_i = findArg("-window_t");
    if (window_title_arg_i >= 0 && window_title_arg_i + 1 < getNumArgs()) {
        app_title = getArg(window_title_arg_i + 1);
    }

    auto create_applicaton_null = [this]() {
        m_app = nullptr;
    };

    auto create_application_gl = [this](int width, int height, const char *title) {
        m_app = new ApplicationGL();
        m_app->initialize(width, height, title);
    };

    auto create_application_d3d = [this, create_application_gl](int width, int height, const char* title) {
        std::cerr << "Direct3D renderer is not currently implemented, using OpenGL instead" << std::endl;
        create_application_gl(width, height, title); // TEMPORARY
    };

    switch (api) {
    case AppAPI::DETECT_MANUALLY:
    {
        int renderer_type_arg_i = findArg("-renderer");
        if (renderer_type_arg_i < 0 || renderer_type_arg_i + 1 == getNumArgs()) {
            create_applicaton_null();
            return;
        } else {
            const char* str_renderer_type = getArg(renderer_type_arg_i + 1);
            if (strcmp(str_renderer_type, "GL") == 0) {
                create_application_gl(app_width, app_height, app_title.c_str());
            } else if (strcmp(str_renderer_type, "D3D") == 0) {
                create_application_d3d(app_width, app_height, app_title.c_str());
            } else {
                create_applicaton_null();
                return;
            }
        }
        break;
    }
    case AppAPI::OPENGL:
        create_application_gl(app_width, app_height, app_title.c_str());
        break;
    case AppAPI::DIRECT3D:
        create_application_d3d(app_width, app_height, app_title.c_str());
        break;
    }

    m_app->setVerticalSync(0);
}

void Engine::initialize_game() {
    Game::get()->initialize();
}

void Engine::update() {
    m_app->preUpdate();

    for (Script* script : m_scripts) {
        script->update();
    }
    Game::get()->update();
    m_app->update();

    for (Script* script : m_scripts) {
        script->render();
    }
    Game::get()->render();
    m_app->postUpdate();
}
