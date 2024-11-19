//
// Created by Zeben on 11/10/2024.
//

#ifndef EWRENDER_TORUSGEN_H
#define EWRENDER_TORUSGEN_H

#pragma once
#include "TorusMesh.h"
#include "../ew/external/glad.h"
#include <GL/gl.h>
#include <vector>
#include <GL/gl.h>

//Major Radius(R): distance from origin to the center of the tube
//Minor Radius(r): radius of the tube
//Sectors: Number of sectors of the tube
//Sides: Number of sides of the tube
//Smooth: Smooth or flat shading
//Up Axis:  Facing Direction, X = 1 Y = 2, Z = 3

class TorusGen {
public:

    //Constructor
    TorusGen(float outerRadius, float innerRadius, int step, int subDivisions);
    //Destructor
    ~TorusGen() = default;

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normals;
        glm::vec2 uvCoordinates;
    };

//draw in vertexArray mode
void draw() const; //draw surface

private:

    //functions
    void buildTorus(float outerRadius, float innerRadius, int steps, int subDivisions);

    //variables
    std::vector<Vertex> torusVertices;
    std::vector<unsigned int> torusIndices;

    unsigned int torusVAO;
};


#endif //EWRENDER_TORUSGEN_H
