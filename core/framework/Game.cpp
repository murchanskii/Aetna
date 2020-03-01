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

void Game::addObjectToScene(Object *obj) {
    GameObject gobject;
    gobject.object = obj;

    m_vec_gobjects.push_back(gobject);
    Engine::get()->getRenderer()->addObjectToRender(obj);
}

void Game::removeObjectFromScene(int num) {
    m_vec_gobjects.erase(m_vec_gobjects.begin() + num);
    Engine::get()->getRenderer()->removeObjectFromRender(getObjectFromScene(num));
}

void Game::removeObjectFromScene(Object *obj) {
    for (int i = 0; i < getNumObjectsInScene(); ++i) {
        if (m_vec_gobjects[i].object == obj) {
            m_vec_gobjects.erase(m_vec_gobjects.begin() + i);
        }
    }
    Engine::get()->getRenderer()->removeObjectFromRender(obj);
}

Object *Game::getObjectFromScene(int num) {
    return m_vec_gobjects[num].object;
}

int Game::findObjectInScene(const char *name) {
    return -1;
}

int Game::getNumObjectsInScene() {
    return m_vec_gobjects.size();
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
    
    for (int i = 0; i < getNumObjectsInScene(); ++i) {
        getObjectFromScene(i)->update();
    }
}

void Game::render() {
    for (GameObject gobject : m_vec_gobjects) {

    }
}
