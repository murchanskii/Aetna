//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_OBJECT_H
#define AETNA_OBJECT_H

#include "Mesh.h"
#include "Material.h"
#include "framework/Game.h"

#include <glm/glm.hpp>

class Object {
public:
    Object();
    explicit Object(Mesh *t_mesh);
    Object(Mesh* t_mesh, Material* t_material);
    virtual ~Object();

    virtual Mesh *getMesh();
    virtual void setMesh(Mesh *t_mesh);

    virtual glm::mat4 getTransform();
    virtual void setTransform(glm::mat4 t_transform);

    virtual glm::vec3 getPosition();
    virtual void setPosition(glm::vec3 pos);

    virtual Material* getMaterial();
    virtual void setMaterial(Material* mat);

    virtual void setEnabled(bool enabled);
    virtual bool isEnabled();

    virtual void setParent(Object *parent);
    virtual Object *getParent();
    virtual void addChild(Object *child);
    virtual Object *getChild(int num);

protected:
    Mesh *m_mesh;
    glm::mat4 m_model;
    Material* m_material;

    friend class Game;
    virtual void update();

    bool is_enabled = true;

    Object *m_parent;
    std::vector<Object*> m_children;
};


#endif //AETNA_OBJECT_H
