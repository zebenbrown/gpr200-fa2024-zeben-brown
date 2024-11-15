//
// Created by Zeben on 11/10/2024.
//

#include "TorusGen.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>


const unsigned int MIN_SECTOR_COUNT = 3;
const unsigned int MIN_SIDE_COUNT = 2;

//Constructor
TorusGen::TorusGen(float outerRadius, float innerRadius, int steps, int subDivisions)
{
    buildTorus(outerRadius, innerRadius, steps, subDivisions);

    //torusVAO;
    glGenVertexArrays(1, &torusVAO);
    glBindVertexArray(torusVAO);

    unsigned int torusVBO;
    glGenBuffers(1, &torusVBO);
    glBindBuffer(GL_ARRAY_BUFFER, torusVBO);
    glBufferData(GL_ARRAY_BUFFER, torusVertices.size() * sizeof(Vertex), torusVertices.data(), GL_STATIC_DRAW);

    unsigned int torusIBO;
    glGenBuffers(1, &torusIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, torusIndices.size() * sizeof(unsigned int), torusIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);

    int stride = sizeof(Vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offsetof(Vertex,Position)));
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

}

void TorusGen::buildTorus(float innerRadius, float outerRadius, int steps, int subDivisions)
{
    const float PI = acos(-1.0f);

    //ring radius(thick part)
    float phi = 0.0;
    float deltaPhi = (2 * PI) / subDivisions;
    //torus radius
    float theta = 0.0;
    float deltaTheta = (2 * PI) / steps;

    for (int stack = 0; stack <= steps; ++stack)
    {
        theta = deltaTheta  * stack;

        for (int slice = 0; slice <= subDivisions; ++slice)
        {
            phi = deltaPhi * slice;

            Vertex vertex;
            vertex.Position.x = cosf(theta) * (outerRadius + cosf(phi) * innerRadius);
            vertex.Position.y = sinf(theta) * (outerRadius + cosf(phi) * innerRadius);
            vertex.Position.z = sinf(phi) * innerRadius;
            torusVertices.push_back(vertex);
        }
    }

    for (int stack = 0; stack <= steps - 1; ++stack)
    {
        for (int slice = 0; slice <= subDivisions; ++slice)
        {
            unsigned int i1 = stack + (slice * subDivisions);
            unsigned int i2 = (stack + 1) + (slice * subDivisions);
            unsigned int i3 = stack + ((slice + 1) * subDivisions);
            unsigned int i4 = (stack + 1) + ((slice + 1) * subDivisions);

            //first triangle
            torusIndices.push_back(i1);
            torusIndices.push_back(i2);
            torusIndices.push_back(i3);

            //second triangle
            torusIndices.push_back(i3);
            torusIndices.push_back(i2);
            torusIndices.push_back(i4);

        }
    }
}

void TorusGen::draw() const
{

    glBindVertexArray(torusVAO);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, torusIndices.size(), GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
}

//Print Self
void TorusGen::printSelf() const
{
    std::cout << "===== Torus =====" << std::endl;
    std::cout << "Major Radius: " << majorRadius << std::endl;
    std::cout << "Minor Radius: " << minorRadius << std::endl;
    std::cout << "Sector Count: " << sectorCount << std::endl;
    std::cout << "Side Count: " << sideCount << sideCount << std::endl;
    std::cout << "Smooth Shading: " << (smooth ? "true" : "false") << std::endl;
    std::cout << "Up Axis: " << (upAxis == 1 ? "X" : (upAxis == 2 ? "Y" : "Z")) << std::endl;
    std::cout << "Triangle Count: " << getTriangleCount() << std::endl;
    std::cout << "Index Count: " << getIndexCount() << std::endl;
    std::cout << "Vertex Count: " << getVertexCount() << std::endl;
    std::cout << "Normal Count: " << getNormalCount() << std::endl;
    std::cout << "Texture Coordinate Count: " << gettextureCoordinatesCount() << std::endl;
}

void TorusGen::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(textureCoordinates);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}