//
// Created by murchanskii on 27.10.2019.
//

#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object() : m_mesh(nullptr), m_material(nullptr), m_parent(nullptr) {
    setTransform(glm::mat4(1.0f));
}

Object::Object(Mesh *t_mesh) : m_material(nullptr), m_parent(nullptr) {
    m_mesh = t_mesh;
    setTransform(glm::mat4(1.0f));
}

Object::Object(Mesh* t_mesh, Material* t_material) : m_parent(nullptr) {
    m_mesh = t_mesh;
    m_material = t_material;
    setTransform(glm::mat4(1.0f));
}

Object::~Object() {

}

void Object::setMesh(Mesh *t_mesh) {
    m_mesh = t_mesh;
}

glm::mat4 Object::getTransform() {
    return m_model;
}

void Object::setTransform(glm::mat4 t_transform) {
    m_model = t_transform;
}

glm::vec3 Object::getPosition() {
    return glm::vec3(m_model[3].x, m_model[3].y, m_model[3].z);
}

void Object::setPosition(glm::vec3 pos) {
    m_model = glm::translate(glm::mat4(1.0f), pos);
}

Material* Object::getMaterial() {
    return m_material;
}

void Object::setMaterial(Material* mat) {
    m_material = mat;
}

void Object::setEnabled(bool enabled) {
    is_enabled = enabled;
}

bool Object::isEnabled() {
    return is_enabled;
}

Mesh *Object::getMesh() {
    return m_mesh;
}

void Object::setParent(Object *parent) {

}

Object *Object::getParent() {
    return nullptr;
}

void Object::addChild(Object *child) {

}

Object *Object::getChild(int num) {
    return nullptr;
}

void Object::update() {
    if (m_material && m_material->getShaderProgram()) {
        m_material->getShaderProgram()->setVariable("transform", &VariableMat4(
            Game::get()->getCamera()->getProjection() * Game::get()->getCamera()->getView() * m_model
        ));
    }
}
