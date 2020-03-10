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
    for (int i = 0; i < m_vec_gents.size(); ++i) {
        if ((m_vec_gents[i].entity->getName(), name) == 0) {
            return i;
        }
    }

    return -1;
}

int Game::findEntityInScene(Entity* entity) {
    for (int i = 0; i < m_vec_gents.size(); ++i) {
        if (m_vec_gents[i].entity == entity) {
            return i;
        }
    }
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
        if (getEntityFromScene(i)->isEnabled()) {

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
}

void Game::render() {
    for (GameEntity gobject : m_vec_gents) {

    }
}

void Game::saveScene(const char* path) {

    auto vec3_to_str = [](glm::vec3 vec3) {
        return std::to_string(vec3.x) + " " + std::to_string(vec3.y) + " " + std::to_string(vec3.z);
    };

    auto vec4_to_str = [](glm::vec4 vec4) {
        return std::to_string(vec4.x) + " " + std::to_string(vec4.y) + " " + std::to_string(vec4.z) + " " + std::to_string(vec4.w);
    };

    auto mat4_to_str = [](glm::mat4 mat4) {
        return std::to_string(mat4[0].x) + " " +
            std::to_string(mat4[0].y) + " " +
            std::to_string(mat4[0].z) + " " +
            std::to_string(mat4[0].w) + " " +

            std::to_string(mat4[1].x) + " " +
            std::to_string(mat4[1].y) + " " +
            std::to_string(mat4[1].z) + " " +
            std::to_string(mat4[1].w) + " " +

            std::to_string(mat4[2].x) + " " +
            std::to_string(mat4[2].y) + " " +
            std::to_string(mat4[2].z) + " " +
            std::to_string(mat4[2].w) + " " +

            std::to_string(mat4[3].x) + " " +
            std::to_string(mat4[3].y) + " " +
            std::to_string(mat4[3].z) + " " +
            std::to_string(mat4[3].w);
    };

    auto vector_float_to_str = [](std::vector<float> vec) {
        std::string result;
        for (int i = 0; i < vec.size(); ++i) {
            result += std::to_string(vec[i]) + " ";
        }
        if (!result.empty()) {
            result = result.erase(result.length() - 1);
        }
        return result;
    };

    pugi::xml_document scene_xml;

    for (int i = 0; i < m_vec_gents.size(); ++i) {
        Entity* entity = m_vec_gents[i].entity;

        pugi::xml_node xml_node_entity = scene_xml.append_child("entity");
        xml_node_entity.append_attribute("id").set_value(i);
        xml_node_entity.append_attribute("name").set_value(entity->getName().c_str());
        xml_node_entity.append_attribute("type").set_value(entity->getTypeName());
        if (entity->isEnabled()) {
            xml_node_entity.append_attribute("enabled").set_value(entity->isEnabled());
        }

        pugi::xml_node xml_node_transform = xml_node_entity.append_child("transform");
        std::string str_world_transform = mat4_to_str(entity->getWorldTransform());            
        xml_node_transform.append_child(pugi::node_pcdata).set_value(str_world_transform.c_str());

        if (dynamic_cast<Object*>(entity)) {
            Object* object = dynamic_cast<Object*>(entity);

            pugi::xml_node xml_node_object = xml_node_entity.append_child("object");
            
            if (object->getMesh()) {
                pugi::xml_node xml_node_mesh = xml_node_object.append_child("mesh");
                pugi::xml_node xml_node_vertex = xml_node_mesh.append_child("vertex");
                xml_node_vertex.append_child(pugi::node_pcdata).set_value(vector_float_to_str(object->getMesh()->getVertices()).c_str());
                pugi::xml_node xml_node_normal = xml_node_mesh.append_child("normal");
                xml_node_normal.append_child(pugi::node_pcdata).set_value(vector_float_to_str(object->getMesh()->getNormals()).c_str());
            }
            
            if (object->getMaterial()) {
                pugi::xml_node xml_node_material = xml_node_object.append_child("material");
                xml_node_material.append_attribute("name").set_value(object->getMaterial()->getName().c_str());
                for (int j = 0;
                    object->getMaterial()->getShaderProgram() && j < object->getMaterial()->getShaderProgram()->getNumVariables();
                    ++j) {
                    pugi::xml_node xml_node_mat_var = xml_node_material.append_child("variable");
                    Variable* var = object->getMaterial()->getShaderProgram()->getVariable(object->getMaterial()->getShaderProgram()->getVariableName(j));
                    xml_node_mat_var.append_attribute("name").set_value(object->getMaterial()->getShaderProgram()->getVariableName(j).c_str());
                    if (var->isInt()) {
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(std::to_string(var->getInt()).c_str());
                    }
                    else if (var->isFloat()) {
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(std::to_string(var->getFloat()).c_str());
                    }
                    else if (var->isVec3()) {
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(vec3_to_str(var->getVec3()).c_str());
                    }
                    else if (var->isVec4()) {
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(vec4_to_str(var->getVec4()).c_str());
                    }
                    else if (var->isMat4()) {
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(mat4_to_str(var->getMat4()).c_str());
                    }
                }
            }

        } else if (dynamic_cast<Light*>(entity)) {
            Light* light = dynamic_cast<Light*>(entity);
            pugi::xml_node xml_node_light = xml_node_entity.append_child("light");
            
            xml_node_light.append_child("ambient").append_child(pugi::node_pcdata).set_value(vec3_to_str(light->getAmbient()).c_str());
            xml_node_light.append_child("diffuse").append_child(pugi::node_pcdata).set_value(vec3_to_str(light->getDiffuse()).c_str());
            xml_node_light.append_child("specular").append_child(pugi::node_pcdata).set_value(vec3_to_str(light->getSpecular()).c_str());
            
            if (dynamic_cast<LightDirectional*>(light)) {
                LightDirectional* light_dir = dynamic_cast<LightDirectional*>(light);
                xml_node_light.append_child("direction").append_child(pugi::node_pcdata).set_value(vec3_to_str(light_dir->getDirection()).c_str());
            }
            else if (dynamic_cast<LightPoint*>(light)) {
                LightPoint* light_point = dynamic_cast<LightPoint*>(light);
                xml_node_light.append_child("constant").append_child(pugi::node_pcdata).set_value(std::to_string(light_point->getConstant()).c_str());
                xml_node_light.append_child("linear").append_child(pugi::node_pcdata).set_value(std::to_string(light_point->getLinear()).c_str());
                xml_node_light.append_child("quadtratic").append_child(pugi::node_pcdata).set_value(std::to_string(light_point->getQuadratic()).c_str());
            }
        }

        if (entity->getParent()) {
            pugi::xml_node xml_node_parent = xml_node_entity.append_child("parent");
            xml_node_parent.append_attribute("id").set_value(findEntityInScene(entity->getParent()));
        }
    }

    scene_xml.save_file(path);
}
