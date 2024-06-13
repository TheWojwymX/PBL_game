#pragma once

#include "texture.h"
#include "Managers/ResourceManager.h"
#include <memory>

class ImageRenderer {
public:

    unsigned int _VBO, _VAO, _EBO;

    int _textureID;
    std::shared_ptr<Shader> _shader;

    array<float, 32> _vertices;

    ImageRenderer();

    void Init(const char* file, array<float, 32> vertices, bool isAlpha, bool isDynamic);
    void Init(const char* file, glm::vec3 initialPosition, bool isAlpha, bool isDynamic);

    void UpdateImage(std::array<float, 32>* vertices = nullptr);

    bool _shouldRender = true;

    void UpdateImage(array<float, 32> *vertices, glm::vec3 additionalColor);

    void Render();

    void AddRotation(float value);
private:
    glm::vec2 _pixelPosition;
    float _rotationAngle;
    glm::mat4 _model;

    float _screenWidth;
    float _screenHeight;

    void UpdateModel();
};
