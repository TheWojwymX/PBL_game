//
// Created by Jacek on 06.04.2024.
//

#ifndef OPENGLGP_IMAGEHUD_H
#define OPENGLGP_IMAGEHUD_H

#include "../../texture.h"
#include "Managers/ResourceManager.h"

class ImageHUD {

public:
    ImageHUD();

    void UpdateImage(std::array<float, 32>* vertices = nullptr);

    unsigned int VBO, VAO, EBO;

    int textureID;

    void Init(const char *file, array<float, 32> vertices, bool isAlpha, bool isDynamic);
};


#endif //OPENGLGP_IMAGEHUD_H
