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
    void setTextureCoordinates(std::vector<float> t_tex_coords);

    std::vector<float> getVertices();
    std::vector<int> getIndices();
    std::vector<float> getNormals();
    std::vector<float> getTextureCoordinates();

private:
    std::vector<float> m_vertices;
    std::vector<int> m_indices;
    std::vector<float> m_normals;
    std::vector<float> m_tex_coords;
};


#endif //AETNA_MESH_H
