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
}

//Setters
/*void TorusGen::set(float majorR, float minorR, int sectors, int sides, bool smooth,
                   int up)
                   {
                        if (majorR > 0)
                            this->majorRadius = majorR;
                        if (minorR > 0)
                            this->minorRadius = minorR;
                        this->sectorCount = sectors;
                        if (sectors < MIN_SECTOR_COUNT)
                            this->sectorCount = MIN_SECTOR_COUNT;
                        this->sideCount = sides;
                        if (sides < MIN_SIDE_COUNT)
                            this->sideCount = MIN_SIDE_COUNT;
                        this->smooth = smooth;
                        this->upAxis = up;
                        if (up < 1 || up > 3)
                            this->upAxis = 3;

                        if (smooth)
                            buildVerticesSmooth();
                        else
                            buildVerticesFlat();
                   }*/

/*void TorusGen::setMajorRadius(float majorRadius)
{
    if (majorRadius != this->majorRadius)
        set(majorRadius, minorRadius, sectorCount, sideCount, smooth, upAxis);
}

void TorusGen::setMinorRadius(float minorRadius)
{
    if (minorRadius != this->minorRadius)
        set(majorRadius, minorRadius, sectorCount, sideCount, smooth, upAxis);
}

void TorusGen::setSectorCount(unsigned int sectors)
{
    if (sectors != this->sectorCount)
        set(majorRadius, minorRadius, sectorCount, sideCount, smooth, upAxis);
}

void  TorusGen::setSideCount(unsigned int sides)
{
    if (sides != this->sideCount)
    {
        set(majorRadius, minorRadius, sectorCount, sideCount, smooth, upAxis);
    }
}

void TorusGen::setSmooth(bool smooth)
{
    if (this->smooth == smooth)
        return;

    this->smooth = smooth;
    if (smooth)
        buildVerticesSmooth();
    else
        buildVerticesFlat();
}*/

//flip the face normals to opposite directions
/*void TorusGen::reverseNormals()
{
    std::size_t i, j;
    std::size_t count = normals.size();
    for (i = 0, j = 3; i < count; i += 3, j += 8)
    {
        normals[i] *= -1;
        normals[i + 1] *= -1;
        normals[i + 2] *= -1;

        interleavedVertices[j] = normals[i];
        interleavedVertices[j + 1] = normals[i + 1];
        interleavedVertices[j + 2] = normals[i + 2];
    }

    unsigned int temp;
    count = indices.size();
    for (int i = 0; i < count; i += 3)
    {
        temp = indices[i];
        indices[i] = indices[i + 2];
        indices[i + 2] = temp;
    }
}*/

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

void TorusGen::draw() const
{
    //interleaveed array
    unsigned int torusVAO;
    glGenVertexArrays(1, &torusVAO);
    glBindVertexArray(torusVAO);

    unsigned int torusVBO;
    glGenBuffers(1, &torusVBO);
    glBindBuffer(GL_ARRAY_BUFFER, torusVBO);
    glBufferData(GL_ARRAY_BUFFER, getInterleavedVertexSize(), getInterleavedVertices(), GL_STATIC_DRAW);

    unsigned int torusIBO;
    glGenBuffers(1, &torusIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexCount() * sizeof(unsigned int), getIndices(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    int stride = getInterleavedStride();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    glBindVertexArray(torusVAO);

    glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
}

void TorusGen::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(textureCoordinates);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

/*void TorusGen::buildVerticesSmooth()
{
    const float PI = acos(-1.0);


    //clear memory of previous arrays
    clearArrays();

    float x, y, z, xy; //vertex position
    float nx, ny, nz; //normal
    float lengthInverse = 1.0f / minorRadius; //to normalize normals
    float s, t; //texture coordinates

    float sectorStep = 2 * PI / sectorCount;
    float sideStep = 2 * PI / sideCount;
    float sectorAngle, sideAngle;

    for (int i = 0; i <= sideCount; ++i)
    {
        //start tube from inside where sideAngle = PI
        sideAngle = PI - i * sideStep; // starting from PI to -PI
        xy = minorRadius * cosf(sideAngle); // r * cos(u)
        z = minorRadius * sinf(sideAngle); // r * sin(u)

        //add (sectorCount + 1) vertices per side
        //the first and last vertices have same position and normal, but different texture coordinates
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; // starting from 0 to 2PI

            //temp x and y to compute normal vector
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            nx = x * lengthInverse;
            ny = y * lengthInverse;
            nz = z * lengthInverse;
            addNormal(nx, ny, nz);

            //shift x and y and vertex position
            x += majorRadius * cosf(sectorAngle); // (R + r * cos(u)) * cos(v)
            y += majorRadius * sinf(sectorAngle); // (R + r * cos(u)) * sin(v)
            addVertex(x, y, z);

            //vertex texture coordinate between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / sideCount;
            addTextureCoordinate(s, t);
        }
        std::cout << "\nVertex: (" << x << ", " << y << ", " << z << ")" << std::endl;
        std::cout << "Normal: (" << nx << ", " << ny << ", " << nz << ")" << std::endl;
    }

    //indices
    //k1--k1+1
    //|  / |
    //| /  |
    //k2--k2+1
    int k1, k2;
    for (int i = 0; i < sideCount; ++i)
    {
        k1 = i * (sectorCount + 1); // beginning of current side
        k2 = k1 + sectorCount + 1; //beginning of next side

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            //2 traingles per sector
            addIndices(k1, k2, k1 + 1); //k1---k2---k1+1
            addIndices(k1+1, k2, k2+1); //k1+1---k2---k2+1

            //vertical lines for all sides
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);

            //horizontal lines for all sides
            lineIndices.push_back(k1);
            lineIndices.push_back(k1 + 1);
        }
    }
    //generate interleaved vertex array
    buildInterleavedVertices();

    //change up axis from Z-axis if given
    if (this->upAxis != 3)
        changeUpAxis(3, this->upAxis);
    }*/

/*void TorusGen::buildVerticesFlat()
{
    const float PI = acos(-1.0f);

    //temp vertex definition (x,y,z,s,t)
    struct Vertex
    {
        float x, y, z, s, t;
    };

    std::vector<Vertex> tempVertices;

    float sectorStep = 2 * PI / sectorCount;
    float sideStep = 2 * PI / sideCount;
    float sectorAngle, sideAngle;

    //compute all vertices first, each vertex contains (x, y, z, s, t) except normal
    for (int i = 0; i <= sideCount; ++i)
    {
        //start the tube side from the inside where sideAngle = PI
        sideAngle = PI - i * sideStep; //starting from PI to -Pi
        float xy = majorRadius + minorRadius * cosf(sideAngle); //R + r * cos(u)
        float z = minorRadius * sinf(sideAngle); //r * sin(u);

        //add (sectorCount + 1) vertices per side
        //the first and last vertices have same position and normal but different texture coordinates

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep; //starting from 0 to 2PI

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle); //x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle); //y = r * cos(u) * sin(v)
            vertex.z = z; //z = r * sin(u)
            vertex.s = (float)j / sectorCount; //s
            vertex.t = (float)i / sideCount; //t
            tempVertices.push_back(vertex);
        }
    }

    //clear memory of previous arrays
    clearArrays();

    Vertex v1, v2, v3, v4; //4 vertex position and texture coordinates
    std::vector<float> normal; //1 face normal

    int i, j, k, vi1, vi2;
    int index = 0; //index for vertex

    for (i = 0; i < sideCount; ++i)
    {
        vi1 = i * (sectorCount + 1); //index of tempVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
            //get 4 vertices per side
            //  v1--v3
            //   |   |
            //  v2--v4
            v1 = tempVertices[vi1];
            v2 = tempVertices[vi2];
            v3 = tempVertices[vi1 + 1];
            v4 = tempVertices[vi2 + 1];

            //store 2 triangles (quad) per side
            //put quad vertices: v1-v2-v3-v4
            addVertex(v1.x, v1.y, v1.z);
            addVertex(v2.x, v2.y, v2.z);
            addVertex(v3.x, v3.y, v3.z);
            addVertex(v4.x, v4.y, v4.z);

            //put texture coordinates of quad
            addTextureCoordinate(v1.s, v1.t);
            addTextureCoordinate(v2.s, v2.t);
            addTextureCoordinate(v3.s, v3.t);
            addTextureCoordinate(v4.s, v4.t);

            //put normal
            normal = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
            for (k = 0; k < 4; ++k) //same normals for 4 vertices
            {
                addNormal(normal[0], normal[1], normal[2]);
            }

            //put indices of quad (2 triangles)
            addIndices(index, index + 1, index + 2);
            addIndices(index + 2, index + 1, index + 3);

            //indices for lines
            lineIndices.push_back(index);
            lineIndices.push_back(index + 1);
            lineIndices.push_back(index);
            lineIndices.push_back(index + 2);

            index += 4; //for next
        }
    }

    //generate interleaved vertex array
    buildInterleavedVertices();

    //change up-axis from z-axis to the given
    if (this->upAxis != 3)
        changeUpAxis(3, this->upAxis);
}*/

/*void TorusGen::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();

    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i + 1]);
        interleavedVertices.push_back(vertices[i + 2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i + 1]);
        interleavedVertices.push_back(normals[i + 2]);

        interleavedVertices.push_back(textureCoordinates[j]);
        interleavedVertices.push_back(textureCoordinates[j + 1]);
    }
}*/

/*void TorusGen::changeUpAxis(int from, int to)
{
    //initial transform matrix columns
    float tx[] = {1.0f, 0.0f, 0.0f}; //x-axis (left)
    float ty[] = {0.0f, 1.0f, 0.0f}; //y-axis (up)
    float tz[] = {0.0f, 0.0f, 1.0f}; //z-axis (forward)

    //X -> Y
    if (from == 1 && to == 2)
    {
        tx[0] = 0.0f;
        tx[1] = 1.0f;

        ty[0] = -1.0;
        ty[1] = 0.0f;
    }

    //X -> Z
    else if (from == 1 && to == 3)
    {
        tx[0] = 0.0f;
        tx[2] = 1.0f;

        tz[0] = -1.0f;
        tz[2] = 0.0f;
    }

    //Y -> Z
    else if (from == 2 && to == 3)
    {
        ty[1] = 0.0f;
        ty[2] = 1.0f;

        tz[1] = -1.0f;
        tz[2] = 0.0f;
    }

    //Z -> X
    else if (from == 3 && to == 1)
    {
        tx[0] = 0.0f;
        tx[2] = -1.0f;

        tz[0] = 1.0f;
        tz[2] = 0.0f;
    }

    //Z -> Y
    else
    {
        ty[1] = 0.0f;
        ty[2] = -1.0f;

        tz[1] = 1.0f;
        tz[2] = 0.0f;
    }

    std::size_t i, j;
    std::size_t count = vertices.size();

    float vertexX, vertexY, vertexZ;
    float normalX, normalY, normalZ;

    for (i = 0, j = 0; i < count; i += 3, j += 8)
    {
        //transform vertices
        vertexX = vertices[i];
        vertexY = vertices[i + 1];
        vertexZ = vertices[i + 2];
        vertices[i] = tx[0] * vertexX + ty[0] * vertexY + tz[0] * vertexZ; //x
        vertices[i + 1] = tx[1] * vertexX + ty[1] * vertexY + tz[1] * vertexZ; //Y
        vertices[i + 2] = tx[2] * vertexX + ty[1] * vertexY + tz[1] * vertexZ; //Z

        //transform normals
        normalX = normals[i];
        normalY = normals[i + 1];
        normalZ = normals[i + 2];
        normals[i] = tx[0] * normalX + ty[0] * normalY + tz[0] * normalZ; //normal x
        normals[i + 1] = tx[1] * normalX + ty[1] * normalY + tz[1] * normalZ; //normal y
        normals[i + 2] = tx[2] * normalX + ty[2] * normalY + tz[2] * normalZ; //normal z

        //transform interleaved array
        interleavedVertices[j] = vertices[i];
        interleavedVertices[j + 1] = vertices[i + 1];
        interleavedVertices[j + 2] = vertices[i + 2];
        interleavedVertices[j + 3] = normals[i];
        interleavedVertices[j + 4] = normals[i + 1];
        interleavedVertices[j + 5] = normals[i + 2];
    }
}*/

/*void TorusGen::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}*/

/*void TorusGen::addNormal(float x, float y, float z)
{
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);
}*/

/*void TorusGen::addTextureCoordinate(float s, float t)
{
    textureCoordinates.push_back(s);
    textureCoordinates.push_back(t);
}*/

/*void TorusGen::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}*/

/*std::vector<float> TorusGen::computeFaceNormal(float x1, float y1, float z1,
                                               float x2, float y2, float z2,
                                               float x3,float y3, float z3)
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f); // default return value (0, 0, 0)
    float normalX, normalY, normalZ;

    //find 2 edge vectors: v1-v2, v-1-v3
    float edgeX1 = x2 - x1;
    float edgeY1 = y2 - y1;
    float edgeZ1 = z2 - z1;
    float edgeX2 = x3 - x1;
    float edgeY2 = y3 - y1;
    float edgeZ2 = z3 - z1;

    //cross product
    normalX = edgeY1 * edgeZ2 - edgeZ1 * edgeY2;
    normalY = edgeZ1 * edgeX2 - edgeX1 * edgeZ2;
    normalZ = edgeX1 * edgeY2 - edgeY1 * edgeX2;

    //normalize only if the length is > 0
    float length = sqrtf(normalX * normalX + normalY * normalY + normalZ * normalZ);
    if (length > EPSILON)
    {
        //normalize
        float lengthInverse = 1.0f / length;
        normal[0] = normalX * lengthInverse;
        normal[1] = normalY * lengthInverse;
        normal[2] = normalZ * lengthInverse;
    }
    return normal;
}*/

void TorusGen::buildTorus(float innerRadius, float outerRadius, int steps, int subDivisions)
{
    const float PI = acos(-1.0f);

    //ring radius(thick part)
    float phi = 0.0;
    float deltaPhi = (2 * PI) / subDivisions;
    //torus radius
    float theta = 0.0;
    float deltaTheta = (2 * PI) / steps;

    //std::vector<glm::vec3> torusVertices;

    for (int stack = 0; stack <= steps; ++stack)
    {
        theta = deltaTheta  * stack;
        for (int slice = 0; slice <= subDivisions; ++slice)
        {
            phi = deltaPhi * slice;
            glm::vec3 v;
            v.x = cosf(theta) * (outerRadius + cosf(phi) * innerRadius);
            v.y = sinf(theta) * (outerRadius + cosf(phi) * innerRadius);
            v.z = sinf(phi) * innerRadius;
            torusVertices.push_back(v);
        }
    }

    //std::vector<unsigned int> torusIndices;
    for (int stack = 0; stack <= steps - 1; ++stack)
    {
        for (int slice = 0; slice <= subDivisions; ++slice)
        {
            unsigned int i1 = stack + (slice + 1);
            unsigned int i2 = (stack + 1) + (slice * subDivisions);
            unsigned int i3 = stack + ((slice + 1) * subDivisions);
            unsigned int i4 = (stack + 1) + ((slice + 1) * subDivisions);

            //first triangle
            torusIndices.push_back(i1);
            torusIndices.push_back(i3);
            torusIndices.push_back(i4);

            //second triangle
            torusIndices.push_back(i1);
            torusIndices.push_back(i4);
            torusIndices.push_back(i2);

        }
    }
}
