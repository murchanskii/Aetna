//
// Created by murchanskii on 27.10.2019.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3> t_vertices, std::vector<int> t_indices) {
    setVertices(t_vertices, t_indices);
}

Mesh::Mesh(std::vector<glm::vec3> t_vertices, std::vector<int> t_indices, std::vector<glm::vec3> t_normals) {
    m_vertices = t_vertices;
    m_indices = t_indices;
    m_normals = t_normals;
}

Mesh::~Mesh() {

}

void Mesh::setVertices(std::vector<glm::vec3> t_vertices, std::vector<int> t_indices) {
    m_vertices = t_vertices;
    m_indices = t_indices;

    if (m_normals.empty()) {
        generate_normals(m_normals);
    }
}

void Mesh::setNormals(std::vector<glm::vec3> t_normals) {
    m_normals = t_normals;
}

void Mesh::generate_normals(std::vector<glm::vec3> &t_normals) {
    auto get_surface_normal = [](const glm::vec3 &p1,
                                       const glm::vec3 &p2,
                                       const glm::vec3 &p3){
        glm::vec3 u = p2 - p1;
        glm::vec3 v = p3 - p1;
        glm::vec3 normal(1.0f);
        normal.x = u.y * v.z - u.z * v.y;
        normal.y = u.z * v.x - u.x * v.z;
        normal.z = u.x * v.y - u.y * v.x;
        return normal;
    };

    if (m_indices.empty()) {
        for (int i = 0; i < m_vertices.size(); i += 3) {
            glm::vec3 normal(1.0f), p1, p2, p3;

            p1 = m_vertices[i];
            p2 = m_vertices[i + 1];
            p3 = m_vertices[i + 2];

            normal = get_surface_normal(p1, p2, p3);
            for (int j = 0; j < 3; ++j) {
                t_normals.emplace_back(normal.x);
                t_normals.emplace_back(normal.y);
                t_normals.emplace_back(normal.z);
            }
        }
    }
    else {
        for (int i = 0; i < m_indices.size(); i += 3) {
            glm::vec3 normal(1.0f), p1, p2, p3;

            p1 = m_vertices[m_indices[i]];
            p2 = m_vertices[m_indices[i + 1]];
            p3 = m_vertices[m_indices[i + 2]];

            normal = get_surface_normal(p1, p2, p3);
            for (int j = 0; j < 3; ++j) {
                t_normals.emplace_back(normal.x);
                t_normals.emplace_back(normal.y);
                t_normals.emplace_back(normal.z);
            }
        }
    }
}

