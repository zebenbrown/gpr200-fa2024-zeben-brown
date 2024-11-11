//
// Created by Zeben on 11/10/2024.
//

#ifndef EWRENDER_TORUSMESH_H
#define EWRENDER_TORUSMESH_H
#pragma once
#include "../ew/ewMath/ewMath.h"
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);
    glm::vec2 uv = glm::vec2(0);

    Vertex(){}
    Vertex(const glm::vec3 &position, const glm::vec3 &normal, glm::vec2 &uv): position(position), normal(normal), uv(uv){}
};

struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    MeshData(){}
};

enum class DrawMode
{
    TRIANGLES = 0,
    POINTS = 1
};

class TorusMesh{
public:
    TorusMesh() {};
    TorusMesh(const MeshData &meshData);
    void load(const MeshData &meshData);
    void draw(DrawMode drawMode = DrawMode::TRIANGLES) const;
    inline int getNumVertices() const { return m_numVertices; }
    inline int getNumIndicies() const { return m_numIndices; }

private:
    bool m_initialized = false;
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    int m_numVertices = 0;
    int m_numIndices = 0;



};


#endif //EWRENDER_TORUSMESH_H
