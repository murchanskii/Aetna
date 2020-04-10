#include "Application.h"

#ifdef _WIN32
#include <Windows.h>
#include <comdef.h>
#endif

Application* Application::instance = nullptr;

Application::Application(Application* app) {
    instance = app;
    m_renderer = nullptr;
    m_input_proxy = nullptr;
}

Application::~Application() {
}

std::string Application::getDirectory() {
#ifdef _WIN32
    WCHAR wpath[MAX_PATH];
    GetModuleFileNameW(NULL, wpath, MAX_PATH);
    _bstr_t conv_path(wpath);
    std::string app_path = conv_path;
    app_path = app_path.substr(0, app_path.rfind('\\'));
    return app_path;
#elif __linux__

#endif
    return std::string();
}

Renderer* Application::getRenderer() {
    return m_renderer;
}
