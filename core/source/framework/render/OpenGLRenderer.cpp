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

            for (int i = 0, tex_num = 0; i < (int)Material::TextureType::COUNT; ++i) {
                Material::TextureType tex_type = (Material::TextureType) i;
                if (!gl_object->scene_object->getMaterial()->getTexture(tex_type)->getData()) {
                    continue;
                }

                OpenGLObject::OpenGLTexture* ogl_tex = new OpenGLObject::OpenGLTexture();
                ogl_tex->num = tex_num++;
                ogl_tex->name = std::string("material.") + gl_object->scene_object->getMaterial()->getTextureName(tex_type);
                glGenTextures(1, &ogl_tex->location);
                glBindTexture(GL_TEXTURE_2D, ogl_tex->location);
                // set the texture wrapping parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // set texture filtering parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // load image, create texture and generate mipmaps
                Texture* tex = gl_object->scene_object->getMaterial()->getTexture(tex_type);
                if (tex) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->getWidth(), tex->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->getData());
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                gl_object->scene_object->getMaterial()->getShaderProgram()->setVariable(ogl_tex->name, &VariableInt(ogl_tex->num));
                gl_object->vec_textures.emplace_back(ogl_tex);
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

            for (int i = GL_TEXTURE0, j = 0; j < gl_object->vec_textures.size() && i <= GL_TEXTURE31; ++i, ++j) {
                glActiveTexture(i);
                glBindTexture(GL_TEXTURE_2D, gl_object->vec_textures[j]->location);
            }

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
