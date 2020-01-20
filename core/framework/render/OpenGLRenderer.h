//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_OPENGLRENDERER_H
#define AETNA_OPENGLRENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "../shader/OpenGLShaderProgram.h"

#include <vector>

class OpenGLRenderer : public Renderer {
public:
    static OpenGLRenderer *get() {
        static OpenGLRenderer instance;
        return &instance;
    }

    void initialize(int window_w, int window_h, const char *window_t) override;
    void update() override;
    void render() override;
    void terminate() override;
    void swap() override;

    bool mainWindowIsOpen() override;

protected:
    struct OpenGLObject {
        Object *scene_object;

        OpenGLShaderProgram *shader_program;
        GLuint VAO;
        std::vector<GLuint> vec_VBOs;
    };

    void addObjectToRender(Object *obj) override;
    void removeObjectFromRender(Object *obj) override;
    void renderObjects() override;

private:
    OpenGLRenderer();
    ~OpenGLRenderer() override;

    bool m_initialized;

    GLFWwindow *m_main_window = nullptr;
    std::vector<OpenGLObject*> m_vec_gl_objects;
};


#endif //AETNA_OPENGLRENDERER_H
