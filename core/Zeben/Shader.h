//
// Created by Zeben on 9/14/2024.
//

#ifndef SHADER_H
#define SHADER_H



 // include glad to get all the required OpenGL headers
#include "..\ew\external\glad.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{

public:
    //The Program ID
    unsigned int ID;

    //Constructor Reads and Builds the Shader
    Shader(const char* vertexPath, const char* fragmentPath);

    //Uses the Shader
    void use();

    //Utility Uniform Functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif SHADER_H
