#include "OpenGLRenderer.h"

#include <glad/glad.h>

#include <framework/Engine.h>

#include <iostream>
#include <algorithm>

OpenGLRenderer::OpenGLRenderer() : m_initialized(false) {

}

OpenGLRenderer::~OpenGLRenderer() {

}

bool OpenGLRenderer::isInitialized() {
    return m_initialized;
}

void OpenGLRenderer::initialize(void *param) {
    if (!gladLoadGLLoader((GLADloadproc)param)) {
        std::cout << "Failed to initialize glad" << std::endl;
        return;
    }
    glViewport(0, 0, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initialize_skybox_buffer();
    initialize_shadow_buffer();
    initialize_gbuffer();
    initialize_depth_buffer();
    initialize_deferred_light_buffer();
    initialize_auxiliary_buffer();
    initialize_postmaterials_buffer();
    initialize_gui_buffer();

    m_initialized = true;
}

void OpenGLRenderer::update() {

}

void OpenGLRenderer::render() {
    glClearColor(.3f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderObjects();


    render_skybox_buffer();
    calculate_geometry();
    render_shadow_buffer();
    render_gbuffer();
    render_depth_buffer();
    render_deferred_light_buffer();
    render_auxiliary_buffer();
    render_postmaterials_buffer();
    render_gui_buffer();
}

void OpenGLRenderer::terminate() {
    for (int i = 0; i < m_vec_gl_objects.size(); ++i) {
        removeObjectFromRender(i);
    }
}

void OpenGLRenderer::addObjectToRender(Object *obj) {
    for (int i = 0; i < m_vec_gl_objects.size(); ++i) {
        if (m_vec_gl_objects[i]->scene_object == obj) {
            return;
        }
    }

    OpenGLObject *gl_object = new OpenGLObject();
    gl_object->scene_object = obj;

    if (gl_object->scene_object->getMesh() && !gl_object->scene_object->getMesh()->getVertices().empty()) {
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

        if (!gl_object->scene_object->getMesh()->getTextureCoordinates().empty()) {
            gl_object->vec_VBOs.push_back(0);
            glGenBuffers(1, &gl_object->vec_VBOs[2]); // texture coordinates

            glBindBuffer(GL_ARRAY_BUFFER, gl_object->vec_VBOs[2]);
            glBufferData(GL_ARRAY_BUFFER,
                gl_object->scene_object->getMesh()->getTextureCoordinates().size() * sizeof(float),
                gl_object->scene_object->getMesh()->getTextureCoordinates().data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
            glDisable(GL_ARRAY_BUFFER);
        }
        glBindVertexArray(0);
    }

    m_vec_gl_objects.push_back(gl_object);
}

void OpenGLRenderer::removeObjectFromRender(int id) {
    OpenGLObject *gl_object = m_vec_gl_objects[id];
    m_vec_gl_objects.erase(m_vec_gl_objects.begin() + id);
    delete gl_object;
}

void OpenGLRenderer::removeObjectFromRender(Object *obj) {
    int id_found = -1;
    for (int i = 0; i < m_vec_gl_objects.size(); ++i) {
        if (obj == m_vec_gl_objects[i]->scene_object) {
            id_found = i;
            break;
        }
    }

    if (id_found == -1) {
        return;
    }

    removeObjectFromRender(id_found);
}

void OpenGLRenderer::renderObjects() {
    for (OpenGLObject *gl_object : m_vec_gl_objects) {
        if (gl_object->scene_object->isEnabled() && gl_object->scene_object->getMaterial()) {
            gl_object->scene_object->getMaterial()->apply();
            glBindVertexArray(gl_object->VAO);
            glDrawArrays(GL_TRIANGLES, 0, gl_object->scene_object->getMesh()->getVertices().size() / 3);
            glBindVertexArray(0);
        }
    }
}

void OpenGLRenderer::resizeViewport(int width, int height) {
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);

    Application::get()->getRenderer()->render();
}

int OpenGLRenderer::getViewportWidth() {
    GLint vp[4]; 
    glGetIntegerv(GL_VIEWPORT, vp);
    return vp[2];
}

int OpenGLRenderer::getViewportHeight() {
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    return vp[3];
}

void OpenGLRenderer::initialize_skybox_buffer() {
}

void OpenGLRenderer::initialize_shadow_buffer() {
}

void OpenGLRenderer::initialize_gbuffer() {
}

void OpenGLRenderer::initialize_depth_buffer() {
}

void OpenGLRenderer::initialize_deferred_light_buffer() {
}

void OpenGLRenderer::initialize_auxiliary_buffer() {
}

void OpenGLRenderer::initialize_postmaterials_buffer() {
}

void OpenGLRenderer::initialize_gui_buffer() {
}

void OpenGLRenderer::render_skybox_buffer() {
}

void OpenGLRenderer::calculate_geometry() {
}

void OpenGLRenderer::render_shadow_buffer() {
}

void OpenGLRenderer::render_gbuffer() {
}

void OpenGLRenderer::render_depth_buffer() {
}

void OpenGLRenderer::render_deferred_light_buffer() {
}

void OpenGLRenderer::render_auxiliary_buffer() {
}

void OpenGLRenderer::render_postmaterials_buffer() {
}

void OpenGLRenderer::render_gui_buffer() {
}
