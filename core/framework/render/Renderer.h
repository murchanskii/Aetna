//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_RENDERER_H
#define AETNA_RENDERER_H

#include "../Game.h"
#include "../../Object.h"

class Renderer {
public:
    Renderer(Renderer const&) = delete;
    Renderer& operator=(Renderer const&) = delete;

    bool isInitialized() {
        return m_is_initialized;
    }

    virtual bool mainWindowIsOpen() = 0;
    virtual void setVerticalSync(int value) = 0;

    virtual void initialize(int window_w, int window_h, const char *window_t) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void terminate() = 0;
    virtual void swap() = 0;

protected:
    friend void Game::addObjectToScene(Object *obj);
    friend void Game::removeObjectFromScene(Object *obj);
    friend void Game::removeObjectFromScene(int num);
    friend void Game::render();

    Renderer() {}
    virtual ~Renderer()  {}

    virtual void addObjectToRender(Object *obj) = 0;
    virtual void removeObjectFromRender(Object *obj) = 0;
    virtual void removeObjectFromRender(int id) = 0;
    virtual void renderObjects() = 0;

private:
    bool m_is_initialized = false;
};

#endif //AETNA_RENDERER_H
