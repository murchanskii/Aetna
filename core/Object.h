//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_OBJECT_H
#define AETNA_OBJECT_H

#include <glm/glm.hpp>
#include "Mesh.h"

class Object {
public:
    Object() = default;
    explicit Object(Mesh *t_mesh);
    ~Object();

    void setMesh(Mesh *t_mesh);
    void setTransform(glm::mat4 &t_transform);

private:
    Mesh *m_mesh;
    glm::mat4 m_transform;
    // todo: material
};


#endif //AETNA_OBJECT_H
