#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Entity::Entity() : m_parent(nullptr) {
    setTransform(glm::mat4(1.0f));
}

Entity::~Entity() {

}

glm::mat4 Entity::getTransform() {
    return m_model;
}

void Entity::setTransform(glm::mat4 t_transform) {
    m_model = t_transform;
}

glm::vec3 Entity::getPosition() {
    return glm::vec3(m_model[3].x, m_model[3].y, m_model[3].z);
}

void Entity::setPosition(glm::vec3 pos) {
    glm::mat4 new_model = glm::mat4(1.0f);
    new_model = glm::translate(new_model, pos) * glm::mat4_cast(getRotation());
    new_model = glm::scale(new_model, getScale());
    m_model = new_model;
}

glm::quat Entity::getRotation() {
    glm::vec3 scale = glm::vec3(
        glm::length(glm::vec3(m_model[0].x, m_model[0].y, m_model[0].z)),
        glm::length(glm::vec3(m_model[1].x, m_model[1].y, m_model[1].z)),
        glm::length(glm::vec3(m_model[2].x, m_model[2].y, m_model[2].z))
    );

    glm::mat4 rotation_mat4 = glm::mat4(0.0f);

    rotation_mat4[0].x = m_model[0].x / scale.x;
    rotation_mat4[0].y = m_model[0].y / scale.x;
    rotation_mat4[0].z = m_model[0].z / scale.x;
    rotation_mat4[0].w = 0.0f;

    rotation_mat4[1].x = m_model[1].x / scale.y;
    rotation_mat4[1].y = m_model[1].y / scale.y;
    rotation_mat4[1].z = m_model[1].z / scale.y;
    rotation_mat4[1].w = 0.0f;

    rotation_mat4[2].x = m_model[2].x / scale.z;
    rotation_mat4[2].y = m_model[2].y / scale.z;
    rotation_mat4[2].z = m_model[2].z / scale.z;
    rotation_mat4[2].w = 0.0f;

    rotation_mat4[3].x = 0.0f;
    rotation_mat4[3].y = 0.0f;
    rotation_mat4[3].z = 0.0f;
    rotation_mat4[3].w = 1.0f;

    glm::quat rotation = glm::quat_cast(rotation_mat4);

	return rotation;
}

void Entity::setRotation(glm::quat rot) {
    glm::mat4 new_model = glm::mat4(1.0f);
    new_model = glm::translate(new_model, getPosition()) * glm::mat4_cast(rot);
    new_model = glm::scale(new_model, getScale());
    m_model = new_model;
}

glm::vec3 Entity::getScale() {
    glm::vec3 scale = glm::vec3(
        glm::length(glm::vec3(m_model[0].x, m_model[0].y, m_model[0].z)),
        glm::length(glm::vec3(m_model[1].x, m_model[1].y, m_model[1].z)),
        glm::length(glm::vec3(m_model[2].x, m_model[2].y, m_model[2].z))
    );
    return scale;
}

void Entity::setScale(glm::vec3 scale) {
    glm::mat4 new_model = glm::mat4(1.0f);
    new_model = glm::translate(new_model, getPosition()) * glm::mat4_cast(getRotation());
    new_model = glm::scale(new_model, scale);
    m_model = new_model;
}

void Entity::setEnabled(bool enabled) {
    is_enabled = enabled;
}

bool Entity::isEnabled() {
    return is_enabled;
}


void Entity::setParent(Entity* parent) {

}

Entity* Entity::getParent() {
    return m_parent;
}

void Entity::addChild(Entity* child) {

}

void Entity::removeChild(Entity* child) {
}

Entity* Entity::getChild(int num) {
    return m_children[num];
}

void Entity::update() {
    
}
