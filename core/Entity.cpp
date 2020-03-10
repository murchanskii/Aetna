#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Entity::Entity() : m_parent(nullptr) {
    setTransform(glm::mat4(1.0f));
}

Entity::~Entity() {

}

const char* Entity::getTypeName() {
    return "Entity";
}

std::string Entity::getName() {
    return m_name;
}

void Entity::setName(std::string name) {
    m_name = name;
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

glm::mat4 Entity::getWorldTransform() {
    if (m_parent) {
        return m_parent->getWorldTransform() * getTransform();
    }
    return getTransform();
}

void Entity::setWorldTransform(glm::mat4 t_transform) {
    if (m_parent) {
        m_model = glm::inverse(m_parent->getWorldTransform()) * t_transform;
    }
    else {
        setTransform(t_transform);
    }
}

glm::vec3 Entity::getWorldPosition() {
    if (m_parent) {
        glm::mat4 world_transform = getWorldTransform();
        return glm::vec3(world_transform[3].x, world_transform[3].y, world_transform[3].z);         
    }
    return getPosition();
}

void Entity::setWorldPosition(glm::vec3 pos) {
    if (m_parent) {
        glm::mat4 new_model = glm::mat4(1.0f);
        new_model = glm::translate(new_model, pos) * glm::mat4_cast(getWorldRotation());
        new_model = glm::scale(new_model, getWorldScale());
        setWorldTransform(new_model);
    }
    else {
        setPosition(pos);
    }
}

glm::quat Entity::getWorldRotation() {
    if (m_parent) {
        glm::mat4 world_transform = getWorldTransform();

        glm::vec3 scale = glm::vec3(
            glm::length(glm::vec3(world_transform[0].x, world_transform[0].y, world_transform[0].z)),
            glm::length(glm::vec3(world_transform[1].x, world_transform[1].y, world_transform[1].z)),
            glm::length(glm::vec3(world_transform[2].x, world_transform[2].y, world_transform[2].z))
        );

        glm::mat4 rotation_mat4 = glm::mat4(0.0f);

        rotation_mat4[0].x = world_transform[0].x / scale.x;
        rotation_mat4[0].y = world_transform[0].y / scale.x;
        rotation_mat4[0].z = world_transform[0].z / scale.x;
        rotation_mat4[0].w = 0.0f;

        rotation_mat4[1].x = world_transform[1].x / scale.y;
        rotation_mat4[1].y = world_transform[1].y / scale.y;
        rotation_mat4[1].z = world_transform[1].z / scale.y;
        rotation_mat4[1].w = 0.0f;

        rotation_mat4[2].x = world_transform[2].x / scale.z;
        rotation_mat4[2].y = world_transform[2].y / scale.z;
        rotation_mat4[2].z = world_transform[2].z / scale.z;
        rotation_mat4[2].w = 0.0f;

        rotation_mat4[3].x = 0.0f;
        rotation_mat4[3].y = 0.0f;
        rotation_mat4[3].z = 0.0f;
        rotation_mat4[3].w = 1.0f;

        return glm::quat_cast(rotation_mat4);
    }
    return getRotation();
}

void Entity::setWorldRotation(glm::quat rot) {
    if (m_parent) {
        glm::mat4 new_model = glm::mat4(1.0f);
        new_model = glm::translate(new_model, getWorldPosition()) * glm::mat4_cast(rot);
        new_model = glm::scale(new_model, getWorldScale());
        setWorldTransform(new_model);
    } else {
        setRotation(rot);
    }
}

glm::vec3 Entity::getWorldScale() {
    if (m_parent) {
        glm::mat4 world_transform = getWorldTransform();
        return glm::vec3(
            glm::length(glm::vec3(world_transform[0].x, world_transform[0].y, world_transform[0].z)),
            glm::length(glm::vec3(world_transform[1].x, world_transform[1].y, world_transform[1].z)),
            glm::length(glm::vec3(world_transform[2].x, world_transform[2].y, world_transform[2].z))
        );
    }
    return getScale();
}

void Entity::setWorldScale(glm::vec3 scale) {
    if (m_parent) {
        glm::mat4 new_model = glm::mat4(1.0f);
        new_model = glm::translate(new_model, getWorldPosition()) * glm::mat4_cast(getWorldRotation());
        new_model = glm::scale(new_model, scale);
        setWorldTransform(new_model);
    }
    else {
        setScale(scale);
    }
}

void Entity::setEnabled(bool enabled) {
    is_enabled = enabled;
}

bool Entity::isEnabled() {
    return is_enabled;
}


void Entity::setParent(Entity* parent) {
    glm::mat4 old_transform = getWorldTransform();
    m_parent = parent;
    setWorldTransform(old_transform);
}

Entity* Entity::getParent() {
    return m_parent;
}

void Entity::addChild(Entity* child) {
    m_children.emplace_back(child);
    child->setParent(this);
}

void Entity::removeChild(Entity* child) {
    m_children.erase(std::find(m_children.begin(), m_children.end(), child));
    child->setParent(nullptr);
}

Entity* Entity::getChild(int num) {
    return m_children[num];
}

void Entity::update() {
    
}
