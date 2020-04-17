#include "Mesh.h"

Mesh::Mesh(std::vector<float> t_vertices, std::vector<int> t_indices) {
    setVertices(t_vertices, t_indices);
}

Mesh::Mesh(std::vector<float> t_vertices, std::vector<int> t_indices, std::vector<float> t_normals) {
    m_vertices = t_vertices;
    m_indices = t_indices;
    m_normals = t_normals;
}

Mesh::~Mesh() {

}

void Mesh::setVertices(std::vector<float> t_vertices, std::vector<int> t_indices) {
    m_vertices = t_vertices;
    m_indices = t_indices;
}

void Mesh::setNormals(std::vector<float> t_normals) {
    m_normals = t_normals;
}

void Mesh::setTextureCoordinates(std::vector<float> t_tex_coords) {
    m_tex_coords = t_tex_coords;
}

std::vector<float> Mesh::getVertices() {
    return m_vertices;
}

std::vector<int> Mesh::getIndices() {
    return m_indices;
}

std::vector<float> Mesh::getNormals() {
    return m_normals;
}

std::vector<float> Mesh::getTextureCoordinates() {
    return m_tex_coords;
}

