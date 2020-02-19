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

    renderObjects();
}

void OpenGLRenderer::terminate() {
    glfwTerminate();
}

bool OpenGLRenderer::mainWindowIsOpen() {
    if (!m_main_window) {
        return false;
    }

    return !glfwWindowShouldClose(m_main_window);
}

void OpenGLRenderer::swap() {
    glfwPollEvents();
    glfwSwapBuffers(m_main_window);
}

void OpenGLRenderer::addObjectToRender(Object *obj) {
    for (int i = 0; i < m_vec_gl_objects.size(); ++i) {
        if (m_vec_gl_objects[i]->scene_object == obj) {
            return;
        }
    }

    OpenGLObject *gl_object = new OpenGLObject();
    gl_object->scene_object = obj;

    if (!gl_object->scene_object->getMesh()->getVertices().empty()) {
        glGenVertexArrays(1, &gl_object->VAO);
        gl_object->vec_VBOs.push_back(0);
        glGenBuffers(1, &gl_object->vec_VBOs[0]); // vertices

        glBindVertexArray(gl_object->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, gl_object->vec_VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER,
                gl_object->scene_object->getMesh()->getVertices().size() * sizeof(float),
                gl_object->scene_object->getMesh()->getVertices().data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glDisable(GL_ARRAY_BUFFER);

        if (!gl_object->scene_object->getMesh()->getNormals().empty()) {
            gl_object->vec_VBOs.push_back(0);
            glGenBuffers(1, &gl_object->vec_VBOs[1]); // normals

            glBindBuffer(GL_ARRAY_BUFFER, gl_object->vec_VBOs[1]);
            glBufferData(GL_ARRAY_BUFFER,
                    gl_object->scene_object->getMesh()->getNormals().size() * sizeof(float),
                    gl_object->scene_object->getMesh()->getNormals().data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glDisable(GL_ARRAY_BUFFER);
        }

        glBindVertexArray(0);
    }

    m_vec_gl_objects.push_back(gl_object);
}

void OpenGLRenderer::removeObjectFromRender(Object *obj) {

}

void OpenGLRenderer::renderObjects() {
    for (OpenGLObject *gl_object : m_vec_gl_objects) {
        gl_object->scene_object->getMaterial()->use();
        glBindVertexArray(gl_object->VAO);
        glDrawArrays(GL_TRIANGLES, 0, gl_object->scene_object->getMesh()->getVertices().size() / 3);
        glBindVertexArray(0);
    }
}

