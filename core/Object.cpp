//
// Created by murchanskii on 27.10.2019.
//

#include "Object.h"

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