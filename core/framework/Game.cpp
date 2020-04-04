//
// Created by Candles on 19.01.2020.
//

#include "Game.h"
#include "Engine.h"
#include "Lights.h"
#include "Materials.h"
#include "objects/PlayerFree.h"

#include <algorithm>

Game::Game() : default_camera(false) {
    setCamera(nullptr);
}

Game::~Game() {
    if (default_camera) {
        delete m_camera;
        m_camera = nullptr;
    }

    while (getNumEntitiesInScene() > 0) {
        Object* obj = dynamic_cast<Object*>(m_vec_gents[0].entity);
        if (obj) {
            Engine::get()->getRenderer()->removeObjectFromRender(obj);
        }

        if (!m_vec_gents[0].is_manual) {
            delete m_vec_gents[0].entity;
            m_vec_gents[0].entity = nullptr;
        }

        m_vec_gents.erase(m_vec_gents.begin());
    }
}

void Game::addEntityToScene(Entity *ent) {
    GameEntity gentity;
    gentity.entity = ent;
    gentity.entity->setId(m_vec_gents.size());
    gentity.is_light = false;
    gentity.is_manual = true;
    
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
    m_vec_gents[num].entity->setId(-1);

    Object* obj = dynamic_cast<Object*>(getEntityFromScene(num));
    if (obj) {
        Engine::get()->getRenderer()->removeObjectFromRender(obj);
    }

    m_vec_gents.erase(m_vec_gents.begin() + num);
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

int Game::findEntityInSceneByName(const char *name) {
    for (int i = 0; i < m_vec_gents.size(); ++i) {
        if (strcmp(m_vec_gents[i].entity->getName().c_str(), name) == 0) {
            return i;
        }
    }

    return -1;
}

int Game::findEntityInSceneById(int id) {
    for (int i = 0; i < m_vec_gents.size(); ++i) {
        if (m_vec_gents[i].entity->getId() == id) {
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
    pugi::xml_document scene_xml;
	pugi::xml_node xml_node_scene = scene_xml.append_child("scene");
    for (int i = 0; i < m_vec_gents.size(); ++i) {
        Entity* entity = m_vec_gents[i].entity;

        pugi::xml_node xml_node_entity = xml_node_scene.append_child("entity");
        xml_node_entity.append_attribute("id").set_value(entity->getId());
        xml_node_entity.append_attribute("name").set_value(entity->getName().c_str());
        xml_node_entity.append_attribute("type").set_value(entity->getTypeName());
        if (entity->isEnabled()) {
            xml_node_entity.append_attribute("enabled").set_value(entity->isEnabled());
        }

        pugi::xml_node xml_node_transform = xml_node_entity.append_child("transform");
        std::string str_world_transform = Utils::mat4ToString(entity->getWorldTransform());            
        xml_node_transform.append_child(pugi::node_pcdata).set_value(str_world_transform.c_str());

        if (dynamic_cast<Object*>(entity)) {
            Object* object = dynamic_cast<Object*>(entity);

            pugi::xml_node xml_node_object = xml_node_entity.append_child("object");
            
            if (object->getMesh()) {
                pugi::xml_node xml_node_mesh = xml_node_object.append_child("mesh");
                pugi::xml_node xml_node_vertex = xml_node_mesh.append_child("vertex");
                xml_node_vertex.append_child(pugi::node_pcdata).set_value(Utils::vecFloatToString(object->getMesh()->getVertices()).c_str());
                pugi::xml_node xml_node_normal = xml_node_mesh.append_child("normal");
                xml_node_normal.append_child(pugi::node_pcdata).set_value(Utils::vecFloatToString(object->getMesh()->getNormals()).c_str());
                pugi::xml_node xml_node_tex_coords = xml_node_mesh.append_child("texture_coordinates");
                xml_node_tex_coords.append_child(pugi::node_pcdata).set_value(Utils::vecFloatToString(object->getMesh()->getTextureCoordinates()).c_str());
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
                        xml_node_mat_var.append_attribute("type").set_value("int");
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(std::to_string(var->getInt()).c_str());
                    }
                    else if (var->isFloat()) {
                        xml_node_mat_var.append_attribute("type").set_value("float");
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(std::to_string(var->getFloat()).c_str());
                    }
                    else if (var->isVec3()) {
                        xml_node_mat_var.append_attribute("type").set_value("vec3");
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(Utils::vec3ToString(var->getVec3()).c_str());
                    }
                    else if (var->isVec4()) {
                        xml_node_mat_var.append_attribute("type").set_value("vec4");
                        xml_node_mat_var.append_child(pugi::node_pcdata).set_value(Utils::vec4ToString(var->getVec4()).c_str());
                    }
                }
            }

        } else if (dynamic_cast<Light*>(entity)) {
            Light* light = dynamic_cast<Light*>(entity);
            pugi::xml_node xml_node_light = xml_node_entity.append_child("light");
            
            xml_node_light.append_child("ambient").append_child(pugi::node_pcdata).set_value(Utils::vec3ToString(light->getAmbient()).c_str());
            xml_node_light.append_child("diffuse").append_child(pugi::node_pcdata).set_value(Utils::vec3ToString(light->getDiffuse()).c_str());
            xml_node_light.append_child("specular").append_child(pugi::node_pcdata).set_value(Utils::vec3ToString(light->getSpecular()).c_str());
            
            if (dynamic_cast<LightDirectional*>(light)) {
                LightDirectional* light_dir = dynamic_cast<LightDirectional*>(light);
                xml_node_light.append_child("direction").append_child(pugi::node_pcdata).set_value(Utils::vec3ToString(light_dir->getDirection()).c_str());
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
            xml_node_parent.append_attribute("id").set_value(entity->getParent()->getId());
        }
    }

    scene_xml.save_file(path);
}

void Game::loadScene(const char* path)
{
	pugi::xml_document scene_xml;
	pugi::xml_parse_result xml_res = scene_xml.load_file(path);
	if (xml_res) {
		pugi::xml_node xml_node_scene = scene_xml.child("scene");
		if (xml_node_scene) {
			for (pugi::xml_node xml_node_entity = xml_node_scene.child("entity");
				xml_node_entity;
				xml_node_entity = xml_node_entity.next_sibling("entity")) {
                const char* name = xml_node_entity.attribute("name").value();
                const char* type = xml_node_entity.attribute("type").value();
                bool enabled = Utils::stringToBool(xml_node_entity.attribute("enabled").value());
                glm::mat4 transform = Utils::stringToMat4(xml_node_entity.child("transform").child_value());

                Entity* entity_to_add = nullptr;
                if (strcmp(type, "Object") == 0) {
                    pugi::xml_node xml_node_object = xml_node_entity.child("object");
                    pugi::xml_node xml_node_mesh = xml_node_object.child("mesh");
                    const char *str_vertex = xml_node_mesh.child("vertex").child_value();
                    const char *str_normal = xml_node_mesh.child("normal").child_value();
                    const char* str_tex_coords = xml_node_mesh.child("texture_coordinates").child_value();
                    Mesh* mesh = new Mesh(Utils::stringToVecFloat(str_vertex));
                    mesh->setNormals(Utils::stringToVecFloat(str_normal));
                    mesh->setTextureCoordinates(Utils::stringToVecFloat(str_tex_coords));

                    pugi::xml_node xml_node_material = xml_node_object.child("material");
                    const char* str_material_name = xml_node_material.attribute("name").value();
                    Material* material = new Material();
                    material->load(Materials::get()->getMaterialPath(str_material_name).c_str());
                    for (pugi::xml_node variable_node = xml_node_material.child("variable");
                        variable_node;
                        variable_node = variable_node.next_sibling("variable")) {
                        Variable* var = nullptr;
                        Utils::parseXmlVariable(variable_node, &var);
                        const char* var_name = variable_node.attribute("name").value();
                        material->getShaderProgram()->setVariable(var_name, var);
                    }

                    Object* object = new Object(mesh, material);
                    entity_to_add = object;
                } else if (std::string(type).rfind("Light", 0) == 0) {
                    pugi::xml_node xml_node_light = xml_node_entity.child("light");
                    if (xml_node_light) {
                        Light* light = nullptr;
                        if (strcmp(type, "LightDirectional") == 0) {
                            LightDirectional* light_dir = new LightDirectional();
                            pugi::xml_node xml_node_direction = xml_node_light.child("direction");
                            if (xml_node_direction) {
                                glm::vec3 direction = Utils::stringToVec3(xml_node_direction.child_value());
                                light_dir->setDirection(direction);
                            }
                            light = light_dir;
                        } else if (strcmp(type, "LightPoint") == 0) {
                            LightPoint* light_pt = new LightPoint();
                            pugi::xml_node xml_node_constant = xml_node_light.child("constant");
                            pugi::xml_node xml_node_linear = xml_node_light.child("linear");
                            pugi::xml_node xml_node_quadtratic = xml_node_light.child("quadtratic");
                            if (xml_node_constant) {
                                float constant = Utils::stringToFloat(xml_node_constant.child_value());
                                light_pt->setConstant(constant);
                            }
                            if (xml_node_linear) {
                                float linear = Utils::stringToFloat(xml_node_linear.child_value());
                                light_pt->setLinear(linear);
                            }
                            if (xml_node_quadtratic) {
                                float quadtratic = Utils::stringToFloat(xml_node_quadtratic.child_value());
                                light_pt->setQuadratic(quadtratic);
                            }
                            light = light_pt;
                        } else if (strcmp(type, "LightSpot") == 0) {
                            LightSpot* light_spt = new LightSpot();
                            light = light_spt;
                        }

                        if (light) {
                            pugi::xml_node xml_node_ambient = xml_node_light.child("ambient");
                            pugi::xml_node xml_node_diffuse = xml_node_light.child("diffuse");
                            pugi::xml_node xml_node_specular = xml_node_light.child("specular");
                            pugi::xml_node xml_node_color = xml_node_light.child("color");

                            if (xml_node_ambient) {
                                glm::vec3 ambient = Utils::stringToVec3(xml_node_ambient.child_value());
                                light->setAmbient(ambient);
                            }
                            if (xml_node_diffuse) {
                                glm::vec3 diffuse = Utils::stringToVec3(xml_node_diffuse.child_value());
                                light->setDiffuse(diffuse);
                            }
                            if (xml_node_specular) {
                                glm::vec3 specular = Utils::stringToVec3(xml_node_specular.child_value());
                                light->setSpecular(specular);
                            }
                            if (xml_node_color) {
                                glm::vec4 color = Utils::stringToVec4(xml_node_color.child_value());
                                light->setColor(color);
                            }

                            entity_to_add = light;
                        }
                    }
                } else if (strcmp(type, "PlayerFree") == 0) {
                    PlayerFree* player = new PlayerFree();
                    entity_to_add = player;
                }

                if (entity_to_add) {
                    entity_to_add->setWorldTransform(transform);
                    entity_to_add->setName(name);
                    entity_to_add->setEnabled(enabled);
                    addEntityToScene(entity_to_add);
                    m_vec_gents.at(findEntityInScene(entity_to_add)).is_manual = false;
                }
			}

            for (pugi::xml_node xml_node_entity = xml_node_scene.child("entity");
                xml_node_entity;
                xml_node_entity = xml_node_entity.next_sibling("entity")) {
                int entity_id = Utils::stringToInt(xml_node_entity.attribute("id").value());
                pugi::xml_node xml_node_parent = xml_node_entity.child("parent");
                if (xml_node_parent) {
                    int parent_id = Utils::stringToInt(xml_node_parent.attribute("id").value());
                    getEntityFromScene(findEntityInSceneById(entity_id))->setParent(getEntityFromScene(findEntityInSceneById(parent_id)));
                }
            }
		}
	}
}
