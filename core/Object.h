//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_OBJECT_H
#define AETNA_OBJECT_H

#include "Mesh.h"
#include "Material.h"
#include "Entity.h"

class Object : public Entity {
public:
    Object();
    explicit Object(Mesh *t_mesh);
    Object(Mesh* t_mesh, Material* t_material);
    virtual ~Object();

    virtual Mesh *getMesh();
    virtual void setMesh(Mesh *t_mesh);
    
    virtual Material* getMaterial();
    virtual void setMaterial(Material* mat);

    virtual void setEnabled(bool enabled);
    virtual bool isEnabled();
    
protected:
    Mesh *m_mesh;
    Material* m_material;

    void update() override;
};


#endif //AETNA_OBJECT_H
