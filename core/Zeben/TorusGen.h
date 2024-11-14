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
    ~TorusGen()
    {
        clearArrays();
    }

//getters
float getMajorRadius() const { return majorRadius; }
float getMinorRadius() const { return minorRadius; }
unsigned int getSectorCount() const { return sectorCount; }
unsigned int getSideCount() const { return sideCount; }

//setters
void set(float majorR, float minorR, int sectors, int sides, bool smooth = true, int up = 3);
void setMajorRadius(float radius);
void setMinorRadius(float radius);
void setSectorCount(unsigned int sectorCount);
void setSideCount(unsigned int sideCount);
void setSmooth(bool smooth);
void setUpAxis(int up);
void reverseNormals();

//getters for vertex data
unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3;}
unsigned int getNormalCount() const { return (unsigned int)normals.size() / 3;}
unsigned int gettextureCoordinatesCount() const { return (unsigned int)textureCoordinates.size() / 2;}
unsigned int getIndexCount() const { return (unsigned int)torusIndices.size(); }
unsigned int getLineIndexCount() const { return (unsigned int)lineIndices.size();}
unsigned int getTriangleCount() const { return getIndexCount() / 3; }
unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
unsigned int getNormalSize() const {return (unsigned int)normals.size() * sizeof(float); }
unsigned int getTextureCoordinateSize() const { return (unsigned int)textureCoordinates.size() * sizeof(float); }
const float* getVertices() const { return vertices.data(); }
const float* getNormals() const { return normals.data(); }
const float* getTextureCoordinates() const { return textureCoordinates.data(); }
const unsigned int* getIndices() const { return torusIndices.data(); }
const unsigned int* getLineIndices() const { return lineIndices.data(); }

//for interleaved vertices: V/N/T
unsigned int getInterleavedVertexCount() const { return getVertexCount(); } //Number of Vertices
unsigned int getInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); } //number of bytes
int getInterleavedStride() const { return interleavedStride; } //should be 32
const float* getInterleavedVertices() const { return interleavedVertices.data(); }

//draw in vertexArray mode
void draw() const; //draw surface
void drawLines(const float lineCOlor[4]) const; //draw lines only
void drawWithLines(const float lineColor[4]) const;// draw surface and lines

//debug
void printSelf() const;


private:

    //functions
    void buildTorus(float outerRadius, float innerRadius, int steps, int subDivisions);
    void buildVerticesSmooth();
    void buildVerticesFlat();
    void buildInterleavedVertices();
    void changeUpAxis(int from, int to);
    void clearArrays();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTextureCoordinate(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,
                                         float x2, float y2, float z2,
                                         float x3, float y3, float z3);


    //variables
    float majorRadius;
    float minorRadius;
    unsigned int sectorCount;
    unsigned int sideCount;
    bool smooth;
    int upAxis;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> textureCoordinates;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;

    //interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;

    //new variables
    std::vector<glm::vec3> torusVertices;
    std::vector<unsigned int> torusIndices;
};


#endif //EWRENDER_TORUSGEN_H
