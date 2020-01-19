//
// Created by murchanskii on 27.10.2019.
//

#include "OpenGLRenderer.h"

#include <iostream>

OpenGLRenderer::OpenGLRenderer(GLFWwindow *t_window) : Renderer(t_window) {
    m_main_window = t_window;
    initialize();
}

OpenGLRenderer::~OpenGLRenderer() {

}


bool OpenGLRenderer::isInitialized() {
    return m_initialized;
}

void OpenGLRenderer::initialize() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init glad" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    int window_w = 0, window_h = 0;
    glfwGetWindowSize(m_main_window, &window_w, &window_h);
    glViewport(0, 0, window_w, window_h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderer::update() {

}

void OpenGLRenderer::render() {
    glClearColor(.3f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::terminate() {
    glfwTerminate();
}

