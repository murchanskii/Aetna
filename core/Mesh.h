//
// Created by murchanskii on 27.10.2019.
//

#ifndef AETNA_MESH_H
#define AETNA_MESH_H

#include <vector>
#include <glm/glm.hpp>

class Mesh {
public:
    explicit Mesh(std::vector<float> t_vertices,
        std::vector<int> t_indices = std::vector<int>());
    Mesh(std::vector<float> t_vertices,
                  std::vector<int> t_indices,
                  std::vector<float> t_normals);
    ~Mesh();

    void setVertices(std::vector<float> t_vertices,
            std::vector<int> t_indices = std::vector<int>());
    void setNormals(std::vector<float> t_normals);

    std::vector<float> getVertices();
    std::vector<int> getIndices();
    std::vector<float> getNormals();

private:
    std::vector<float> m_vertices;
    std::vector<int> m_indices;
    std::vector<float> m_normals;

    void generate_normals(std::vector<float> &t_normals);
};


#endif //AETNA_MESH_H
