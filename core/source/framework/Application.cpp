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

const char* Application::getDirectory() {
    if (!m_app_path.empty()) {
        return m_app_path.c_str();
    }

#ifdef _WIN32
    WCHAR wpath[MAX_PATH];
    GetModuleFileNameW(NULL, wpath, MAX_PATH);
    _bstr_t conv_path(wpath);
    m_app_path = conv_path;
    m_app_path = m_app_path.substr(0, m_app_path.rfind('\\'));
    return m_app_path.c_str();
#elif __linux__

#endif
    return m_app_path.c_str();
}

Renderer* Application::getRenderer() {
    return m_renderer;
}
