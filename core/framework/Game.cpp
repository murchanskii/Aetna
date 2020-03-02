//
// Created by Candles on 19.01.2020.
//

#include "Game.h"
#include "Engine.h"
#include "Lights.h"

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
    gentity.is_light = false;
    
    Object* obj = dynamic_cast<Object*>(ent);
    if (obj) {
        Engine::get()->getRenderer()->addObjectToRender(obj);
    }

    Light* light = dynamic_cast<Light*>(ent);
    if (light) {
        int last_id = m_vec_gents.size();
        if (dynamic_cast<LightDirectional*>(light)) {
            m_vec_light_dir_indices.push_back(last_id);
        }
        else if (dynamic_cast<LightPoint*>(light)) {
            m_vec_light_point_indices.push_back(last_id);
        }
        else if (dynamic_cast<LightSpot*>(light)) {
            m_vec_light_spot_indices.push_back(last_id);
        }
        gentity.is_light = true;
    }
    
    m_vec_gents.push_back(gentity);
}

void Game::removeEntityFromScene(int num) {
    m_vec_gents.erase(m_vec_gents.begin() + num);

    Object* obj = dynamic_cast<Object*>(getEntityFromScene(num));
    if (obj) {
        Engine::get()->getRenderer()->removeObjectFromRender(obj);
    }

    std::vector<int>::iterator iter_light = std::find(m_vec_light_dir_indices.begin(), m_vec_light_dir_indices.end(), num);
    if (iter_light != m_vec_light_dir_indices.end()) {
        m_vec_light_dir_indices.erase(iter_light);
    }
    iter_light = std::find(m_vec_light_point_indices.begin(), m_vec_light_point_indices.end(), num);
    if (iter_light != m_vec_light_point_indices.end()) {
        m_vec_light_point_indices.erase(iter_light);
    }
    iter_light = std::find(m_vec_light_spot_indices.begin(), m_vec_light_spot_indices.end(), num);
    if (iter_light != m_vec_light_spot_indices.end()) {
        m_vec_light_spot_indices.erase(iter_light);
    }
}

void Game::removeEntityFromScene(Entity *ent) {
    for (int i = 0; i < getNumEntitiesInScene(); ++i) {
        if (m_vec_gents[i].entity == ent) {
            removeEntityFromScene(i);
            break;
        }
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
        
        Object* obj = dynamic_cast<Object*>(getEntityFromScene(i));
        if (obj && obj->getMaterial() && obj->getMaterial()->getShaderProgram()) {
            obj->getMaterial()->getShaderProgram()->setVariable(
                std::string("view_position"),
                &VariableVec3(getCamera()->getPosition()));
        }

        if (!m_vec_gents[i].is_light) {
            for (int j = 0; j < getNumEntitiesInScene(); ++j) {
                Entity* check_entity = getEntityFromScene(j);
                LightDirectional* light_dir = dynamic_cast<LightDirectional*>(check_entity);
                if (light_dir) {
                    int light_dir_index = std::distance(m_vec_light_dir_indices.begin(), 
                        std::find(m_vec_light_dir_indices.begin(), m_vec_light_dir_indices.end(), j));
                    if (obj && obj->getMaterial() && obj->getMaterial()->getShaderProgram()) {
                        obj->getMaterial()->getShaderProgram()->setVariable(
                            std::string("n_lights_directional"),
                            &VariableInt(m_vec_light_dir_indices.size()));
                        obj->getMaterial()->getShaderProgram()->setVariable(
                            std::string("lights_directional[" + std::to_string(light_dir_index) + "].direction"),
                            &VariableVec3(light_dir->getDirection()));
                        obj->getMaterial()->getShaderProgram()->setVariable(
                            std::string("lights_directional[" + std::to_string(light_dir_index) + "].ambient"),
                            &VariableVec3(light_dir->getAmbient()));
                        obj->getMaterial()->getShaderProgram()->setVariable(
                            std::string("lights_directional[" + std::to_string(light_dir_index) + "].diffuse"),
                            &VariableVec3(light_dir->getDiffuse()));
                        obj->getMaterial()->getShaderProgram()->setVariable(
                            std::string("lights_directional[" + std::to_string(light_dir_index) + "].specular"),
                            &VariableVec3(light_dir->getSpecular()));
                        obj->getMaterial()->getShaderProgram()->setVariable(
                            std::string("lights_directional[" + std::to_string(light_dir_index) + "].light_color"),
                            &VariableVec4(light_dir->getColor()));
                    }
                } else {
                    LightPoint* light_point = dynamic_cast<LightPoint*>(check_entity);
                    if (light_point) {
                        int light_point_index = std::distance(m_vec_light_point_indices.begin(),
                            std::find(m_vec_light_point_indices.begin(), m_vec_light_point_indices.end(), j));
                        Object* obj = dynamic_cast<Object*>(getEntityFromScene(i));
                        if (obj && obj->getMaterial() && obj->getMaterial()->getShaderProgram()) {
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("n_lights_point"),
                                &VariableInt(m_vec_light_point_indices.size()));
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("lights_point[" + std::to_string(light_point_index) + "].position"),
                                &VariableVec3(light_point->getPosition()));
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("lights_point[" + std::to_string(light_point_index) + "].ambient"),
                                &VariableVec3(light_point->getAmbient()));
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("lights_point[" + std::to_string(light_point_index) + "].diffuse"),
                                &VariableVec3(light_point->getDiffuse()));
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("lights_point[" + std::to_string(light_point_index) + "].specular"),
                                &VariableVec3(light_point->getSpecular()));
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("lights_point[" + std::to_string(light_point_index) + "].light_color"),
                                &VariableVec4(light_point->getColor()));
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("lights_point[" + std::to_string(light_point_index) + "].constant"),
                                &VariableFloat(light_point->getConstant()));
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("lights_point[" + std::to_string(light_point_index) + "].linear"),
                                &VariableFloat(light_point->getLinear()));
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("lights_point[" + std::to_string(light_point_index) + "].quadratic"),
                                &VariableFloat(light_point->getQuadratic()));
                        }
                    } else {
                        LightSpot* light_spot = dynamic_cast<LightSpot*>(check_entity);
                        if (light_spot) {
                            obj->getMaterial()->getShaderProgram()->setVariable(
                                std::string("n_lights_spot"),
                                &VariableInt(m_vec_light_spot_indices.size()));
                        }
                    }
                }
            }
        }
    }
}

void Game::render() {
    for (GameEntity gobject : m_vec_gents) {

    }
}
