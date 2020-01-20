//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_OBJECT_H
#define AETNA_OBJECT_H

#include <glm/glm.hpp>
#include "Mesh.h"

class Object {
public:
    Object();
    explicit Object(Mesh *t_mesh);
    virtual ~Object();

    Mesh *getMesh();
    void setMesh(Mesh *t_mesh);
    void setTransform(glm::mat4 &t_transform);

    void setEnabled(bool enabled);
    bool isEnabled();

    void setParent(Object *parent);
    Object *getParent();
    void addChild(Object *child);
    Object *getChild(int num);

private:
    Mesh *m_mesh;
    glm::mat4 m_transform;
    // todo: material

    bool is_enabled = true;

    Object *m_parent;
    std::vector<Object*> m_children;
};


#endif //AETNA_OBJECT_H
