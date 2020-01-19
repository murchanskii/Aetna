//
// Created by murchanskii on 27.10.2019.
//

#include "OpenGLRenderer.h"

#include <iostream>

OpenGLRenderer::OpenGLRenderer()  {

}

OpenGLRenderer::~OpenGLRenderer() {

}

void OpenGLRenderer::initialize(int window_w, int window_h, const char *window_t) {
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    m_main_window = glfwCreateWindow(window_w, window_h,
                                     window_t, nullptr, nullptr);
    if (m_main_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(m_main_window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init glad" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    int w_w = 0, w_h = 0;
    glfwGetWindowSize(m_main_window, &w_w, &w_h);
    glViewport(0, 0, window_w, window_h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_initialized = true;
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

bool OpenGLRenderer::mainWindowIsOpen() {
    return !glfwWindowShouldClose(m_main_window);
}

void OpenGLRenderer::swap() {
    glfwPollEvents();
    glfwSwapBuffers(m_main_window);
}

