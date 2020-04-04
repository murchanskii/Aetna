//
// Created by murchanskii on 27.10.2019.
//

#include "OpenGLRenderer.h"

#include <iostream>
#include <algorithm>

OpenGLRenderer::OpenGLRenderer() : m_initialized(false) {

}

OpenGLRenderer::~OpenGLRenderer() {

}

void OpenGLRenderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
    
    OpenGLRenderer::get()->render();
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

    glfwSetFramebufferSizeCallback(m_main_window, framebuffer_size_callback);

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
    for (int i = 0; i < m_vec_gl_objects.size(); ++i) {
        removeObjectFromRender(i);
    }
    glfwTerminate();
}

GLFWwindow* OpenGLRenderer::getMainWindow() {
    return m_main_window;
}

bool OpenGLRenderer::mainWindowIsOpen() {
    if (!m_main_window) {
        return false;
    }

    return !glfwWindowShouldClose(m_main_window);
}

void OpenGLRenderer::setVerticalSync(int value) {
    glfwSwapInterval(value);
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

            if (gl_object->scene_object->getMaterial()->getTextureAlbedo()) {
                OpenGLObject::OpenGLTexture *ogl_albedo_tex = new OpenGLObject::OpenGLTexture();
                ogl_albedo_tex->num = 0;
                ogl_albedo_tex->name = "u_albedo_texture";
                glGenTextures(1, &ogl_albedo_tex->location);
                glBindTexture(GL_TEXTURE_2D, ogl_albedo_tex->location);
                // set the texture wrapping parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // set texture filtering parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // load image, create texture and generate mipmaps
                Texture* albedo_tex = gl_object->scene_object->getMaterial()->getTextureAlbedo();
                if (albedo_tex) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, albedo_tex->getWidth(), albedo_tex->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, albedo_tex->getData());
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                gl_object->scene_object->getMaterial()->getShaderProgram()->setVariable(ogl_albedo_tex->name, &VariableInt(ogl_albedo_tex->num));
                gl_object->vec_textures.emplace_back(ogl_albedo_tex);
            }
        }

        glBindVertexArray(0);
    }

    m_vec_gl_objects.push_back(gl_object);
}

void OpenGLRenderer::removeObjectFromRender(int id) {
    OpenGLObject *gl_object = m_vec_gl_objects[id];
    m_vec_gl_objects.erase(m_vec_gl_objects.begin() + id);
    while (!gl_object->vec_textures.empty()) {
        OpenGLObject::OpenGLTexture* ogl_texture = gl_object->vec_textures[0];
        gl_object->vec_textures.erase(gl_object->vec_textures.begin());
        delete ogl_texture;
    }
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

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gl_object->vec_textures[0]->location); // albedo

            gl_object->scene_object->getMaterial()->apply();
            glBindVertexArray(gl_object->VAO);
            glDrawArrays(GL_TRIANGLES, 0, gl_object->scene_object->getMesh()->getVertices().size() / 3);
            glBindVertexArray(0);
        }
    }
}

void OpenGLRenderer::resizeWindow(int width, int height) {
    glfwSetWindowSize(m_main_window, width, height);
}

int OpenGLRenderer::getWindowWidth() {
    int width = 0, height = 0;
    glfwGetWindowSize(m_main_window, &width, &height);
    return width;
}

int OpenGLRenderer::getWindowHeight() {
    int width = 0, height = 0;
    glfwGetWindowSize(m_main_window, &width, &height);
    return height;
}
