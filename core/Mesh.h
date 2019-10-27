//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA2D_MESH_H
#define AETNA2D_MESH_H

#include <vector>
#include <glm/glm.hpp>

class Mesh {
public:
    explicit Mesh(std::vector<glm::vec3> t_vertices,
        std::vector<int> t_indices = std::vector<int>());
    Mesh(std::vector<glm::vec3> t_vertices,
                  std::vector<int> t_indices,
                  std::vector<glm::vec3> t_normals);
    ~Mesh();

    void setVertices(std::vector<glm::vec3> t_vertices,
            std::vector<int> t_indices = std::vector<int>());
    void setNormals(std::vector<glm::vec3> t_normals);

private:
    std::vector<glm::vec3> m_vertices;
    std::vector<int> m_indices;
    std::vector<glm::vec3> m_normals;

    void generate_normals(std::vector<glm::vec3> &t_normals);
};


#endif //AETNA2D_MESH_H
