//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_RENDERER_H
#define AETNA_RENDERER_H

#include "../Game.h"
#include "../../Object.h"
#include "../Engine.h"

#include <iostream>

class Renderer {
public:
    Renderer(Renderer const&) = delete;
    Renderer& operator=(Renderer const&) = delete;

    bool isInitialized() {
        return m_is_initialized;
    }

    virtual bool mainWindowIsOpen() = 0;
    virtual void setVerticalSync(int value) = 0;

	virtual void initialize(int window_w, int window_h, const char* window_t) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void terminate() = 0;
    virtual void swap() = 0;
    
protected:
    //friend void Game::addEntityToScene(Entity *ent);
    //friend void Game::removeEntityFromScene(Entity *ent);
    //friend void Game::removeEntityFromScene(int num);
    //friend void Game::render();
    friend class Game;// ::~Game();

    friend class Engine;

    Renderer() {}
    virtual ~Renderer()  {}
    
    virtual void addObjectToRender(Object* obj) = 0;
    virtual void removeObjectFromRender(Object* obj) = 0;
    virtual void removeObjectFromRender(int id) = 0;
    virtual void renderObjects() = 0;

    virtual void resizeWindow(int width, int height) = 0;
    virtual int getWindowHeight() = 0;
    virtual int getWindowWidth() = 0;
    
private:
    bool m_is_initialized = false;
};

#endif //AETNA_RENDERER_H
