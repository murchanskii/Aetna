//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA2D_RENDERER_H
#define AETNA2D_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
    explicit Renderer(GLFWwindow* t_window) {};
    virtual ~Renderer() = default;

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void terminate() = 0;
};

#endif //AETNA2D_RENDERER_H
