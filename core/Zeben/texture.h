//
// Created by Zeben on 9/30/2024.
//

#ifndef EWRENDER_TEXTURE_H
#define EWRENDER_TEXTURE_H


#include <string>
#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

class texture {

public:
    unsigned int load2DTexture(unsigned int &textureName, const char* fileName);


};


#endif //EWRENDER_TEXTURE_H
