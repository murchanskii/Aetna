//
// Created by murchanskii on 27.10.2019.
//

#include "Engine.h"
#include "render/OpenGLRenderer.h"

#include <iostream>

Engine::Engine() {
    m_default = true;
    init();
}

Engine::Engine(Renderer *t_renderer) {
    m_default = false;
    init();
}

Engine::~Engine() {
    if (m_default) {
        delete m_renderer;
        //delete m_main_window;
    }
}

void Engine::init() {
    if (m_default) {
        if (!glfwInit()) {
            std::cout << "Failed to init GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        GLuint window_w = 1000, window_h = 1000;
        std::string window_title = "Aetna2D";
        m_main_window = glfwCreateWindow(window_w, window_h,
                                              window_title.c_str(), nullptr, nullptr);
        if (m_main_window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(m_main_window);

        m_renderer = new OpenGLRenderer(m_main_window);
    }
}

void Engine::update() {
    m_renderer->update();

    m_renderer->render();
}

void Engine::terminate() {
    glfwTerminate();
}

void Engine::run() {
    while (!glfwWindowShouldClose(m_main_window)) {
        update();
        glfwPollEvents();
        glfwSwapBuffers(m_main_window);
    }

    m_renderer->terminate();
    terminate();
}
