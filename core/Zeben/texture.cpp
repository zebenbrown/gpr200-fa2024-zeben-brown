//
// Created by Zeben on 9/30/2024.
//

#include <iostream>
#include "texture.h"


unsigned int texture::load2DTexture(unsigned int &textureName, const char *fileName) {

    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int height, width, nrChannels;
    unsigned char *picture = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (picture)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, picture);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load " << fileName << std::endl;
        std::cout << "Failed " << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(picture);

    return textureName;
}