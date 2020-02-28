//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_OBJECT_H
#define AETNA_OBJECT_H

#include "Mesh.h"
#include "Material.h"

#include <glm/glm.hpp>

class Object {
public:
    Object();
    explicit Object(Mesh *t_mesh);
    Object(Mesh* t_mesh, Material* t_material);
    virtual ~Object();

    Mesh *getMesh();
    void setMesh(Mesh *t_mesh);

    glm::mat4 getTransform();
    void setTransform(glm::mat4 &t_transform);

    Material* getMaterial();
    void setMaterial(Material* mat);

    void setEnabled(bool enabled);
    bool isEnabled();

    void setParent(Object *parent);
    Object *getParent();
    void addChild(Object *child);
    Object *getChild(int num);

protected:
    Mesh *m_mesh;
    glm::mat4 m_model;
    Material* m_material;

    bool is_enabled = true;

    Object *m_parent;
    std::vector<Object*> m_children;
};


#endif //AETNA_OBJECT_H
