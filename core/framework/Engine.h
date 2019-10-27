//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA2D_ENGINE_H
#define AETNA2D_ENGINE_H

#include "render/Renderer.h"

class Engine {
public:
    Engine();
    explicit Engine(Renderer *t_renderer);
    ~Engine();

    void run();

private:
    bool m_default;

    GLFWwindow *m_main_window;
    Renderer *m_renderer;

    void init();
    void update();
    void terminate();
};


#endif //AETNA2D_ENGINE_H
