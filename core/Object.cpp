//
// Created by murchanskii on 27.10.2019.
//

#include "Object.h"

Object::Object() : m_mesh(nullptr) {

}

Object::Object(Mesh *t_mesh) {
    m_mesh = t_mesh;
}

Object::~Object() {

}

void Object::setMesh(Mesh *t_mesh) {
    m_mesh = t_mesh;
}

void Object::setTransform(glm::mat4 &t_transform) {
    m_transform = t_transform;
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
