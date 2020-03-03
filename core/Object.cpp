//
// Created by murchanskii on 27.10.2019.
//

#include "Object.h"

Object::Object() : Entity(), m_mesh(nullptr), m_material(nullptr) {

}

Object::Object(Mesh *t_mesh) : Entity(), m_material(nullptr) {
    m_mesh = t_mesh;
}

Object::Object(Mesh* t_mesh, Material* t_material) : Entity() {
    m_mesh = t_mesh;
    m_material = t_material;
}

Object::~Object() {

}

void Object::setMesh(Mesh *t_mesh) {
    m_mesh = t_mesh;
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

void Object::update() {
    if (m_material && m_material->getShaderProgram()) {
        m_material->getShaderProgram()->setVariable("model", &VariableMat4(getWorldTransform()));
        m_material->getShaderProgram()->setVariable("view", &VariableMat4(Game::get()->getCamera()->getView()));
        m_material->getShaderProgram()->setVariable("projection", &VariableMat4(Game::get()->getCamera()->getProjection()));
    }
}
