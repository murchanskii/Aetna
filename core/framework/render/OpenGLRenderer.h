//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_OPENGLRENDERER_H
#define AETNA_OPENGLRENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"

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

    bool mainWindowIsOpen() override;

    void swap() override;

private:
    OpenGLRenderer();
    ~OpenGLRenderer() override;

    bool m_initialized;

    GLFWwindow *m_main_window;
};


#endif //AETNA_OPENGLRENDERER_H
