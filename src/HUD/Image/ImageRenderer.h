//
// Created by Jacek on 06.04.2024.
//

#ifndef OPENGLGP_IMAGEHUD_H
#define OPENGLGP_IMAGEHUD_H

#include "../../texture.h"
#include "Managers/ResourceManager.h"

class ImageRenderer {

public:

    unsigned int _VBO, _VAO, _EBO;

    int _textureID;

    array<float, 32> _vertices;

    ImageRenderer();

    void Init(const char *file, array<float, 32> vertices, bool isAlpha, bool isDynamic);

    bool _shouldRender = true;

    float _rotationAngle = 0.0f;

    void UpdateImage(array<float, 32> *vertices, glm::vec3 *additionalColor);
};


#endif //OPENGLGP_IMAGEHUD_H
