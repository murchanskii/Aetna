//
// Created by Candles on 19.01.2020.
//

#include "Game.h"
#include "Engine.h"
#include <algorithm>

Game::Game() : default_camera(false) {
    setCamera(nullptr);
}

Game::~Game() {
    if (default_camera) {
        delete m_camera;
        m_camera = nullptr;
    }
}

void Game::addEntityToScene(Entity *ent) {
    GameEntity gentity;
    gentity.entity = ent;

    m_vec_gents.push_back(gentity);

    Object* obj = dynamic_cast<Object*>(ent);
    if (obj) {
        Engine::get()->getRenderer()->addObjectToRender(obj);
    }
}

void Game::removeEntityFromScene(int num) {
    m_vec_gents.erase(m_vec_gents.begin() + num);

    Object* obj = dynamic_cast<Object*>(getEntityFromScene(num));
    if (obj) {
        Engine::get()->getRenderer()->removeObjectFromRender(obj);
    }
}

void Game::removeEntityFromScene(Entity *ent) {
    for (int i = 0; i < getNumEntitiesInScene(); ++i) {
        if (m_vec_gents[i].entity == ent) {
            m_vec_gents.erase(m_vec_gents.begin() + i);
        }
    }

    Object* obj = dynamic_cast<Object*>(ent);
    if (obj) {
        Engine::get()->getRenderer()->removeObjectFromRender(obj);
    }
}

Entity *Game::getEntityFromScene(int num) {
    return m_vec_gents[num].entity;
}

int Game::findEntityInScene(const char *name) {
    return -1;
}

int Game::getNumEntitiesInScene() {
    return m_vec_gents.size();
}

Camera* Game::getCamera() {
    return m_camera;
}

void Game::setCamera(Camera* cam) {
    if (default_camera) {
        delete m_camera;
        m_camera = nullptr;
    }

    if (!cam) {
        m_camera = new Camera();
    }
    else {
        m_camera = cam;
    }

    default_camera = !cam;
}

void Game::update() {
    m_camera->update();
    
    for (int i = 0; i < getNumEntitiesInScene(); ++i) {
        getEntityFromScene(i)->update();
    }
}

void Game::render() {
    for (GameEntity gobject : m_vec_gents) {

    }
}
