//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA2D_OPENGLRENDERER_H
#define AETNA2D_OPENGLRENDERER_H

#include <glad/glad.h>
#include "Renderer.h"

class OpenGLRenderer : public Renderer {
public:
    explicit OpenGLRenderer(GLFWwindow* t_window);
    ~OpenGLRenderer() override;

    bool isInitialized();

    void initialize() override;
    void update() override;
    void render() override;
    void terminate() override;

private:
    bool m_initialized;

    GLFWwindow *m_main_window;
};


#endif //AETNA2D_OPENGLRENDERER_H
