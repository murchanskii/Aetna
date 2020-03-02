#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

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
    m_model = glm::translate(glm::mat4(1.0f), pos);
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
