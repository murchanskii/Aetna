//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_ENTITY_H
#define AETNA_ENTITY_H

#include "framework/Game.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

class Entity {
public:
    Entity();
    virtual ~Entity();

    int getId();

    virtual std::string getName();
    virtual void setName(std::string name);

    virtual const char *getTypeName();
    
    virtual glm::mat4 getTransform();
    virtual void setTransform(glm::mat4 t_transform);

    virtual glm::vec3 getPosition();
    virtual void setPosition(glm::vec3 pos);

    virtual glm::quat getRotation();
    virtual void setRotation(glm::quat rot);

    virtual glm::vec3 getScale();
    virtual void setScale(glm::vec3 scale);

    virtual glm::mat4 getWorldTransform();
    virtual void setWorldTransform(glm::mat4 t_transform);

    virtual glm::vec3 getWorldPosition();
    virtual void setWorldPosition(glm::vec3 pos);

    virtual glm::quat getWorldRotation();
    virtual void setWorldRotation(glm::quat rot);

    virtual glm::vec3 getWorldScale();
    virtual void setWorldScale(glm::vec3 scale);
    
    virtual void setEnabled(bool enabled);
    virtual bool isEnabled();

    virtual void setParent(Entity* parent);
    virtual Entity* getParent();
    virtual void addChild(Entity* child);
    virtual void removeChild(Entity *child);
    virtual Entity* getChild(int num);

protected:
    int m_id;
    std::string m_name;
    glm::mat4 m_model;

    friend class Game;
    virtual void update();

    void setId(int id);

    bool is_enabled = true;

    Entity* m_parent;
    std::vector<Entity*> m_children;
};


#endif //AETNA_ENTITY_H
