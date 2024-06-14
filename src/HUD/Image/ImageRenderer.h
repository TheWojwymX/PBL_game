#pragma once

#include "texture.h"
#include "Managers/ResourceManager.h"
#include <memory>
#include <utility>

class ImageRenderer {
public:

    unsigned int _VBO, _VAO, _EBO;

    int _textureID;
    std::shared_ptr<Shader> _shader;

    array<float, 32> _vertices;

    ImageRenderer();

    void Init(const char* file, array<float, 32> vertices, bool isAlpha, bool isDynamic);
    void Init(const char* file, glm::vec2 initialPosition, float initialRotation, bool isAlpha, bool isDynamic);
    void Init(const char* file, glm::vec2 downLeftCorner, glm::vec2 topRightCorner, bool isAlpha, bool isDynamic);

    void UpdateImage(std::array<float, 32>* vertices = nullptr);

    bool _shouldRender = true;

    void UpdateImage(array<float, 32> *vertices, glm::vec3 additionalColor);

    void Render();

    void AddRotation(float value);
    glm::vec2 CalculateCenter();
    std::pair<glm::vec2, glm::vec2> GetCorners();
private:
    glm::vec2 _pixelPosition;
    float _rotationAngle;
    glm::mat4 _model;
    std::array<float, 20> _vertices2;

    float _screenWidth;
    float _screenHeight;

    void UpdateModel();
    void SetupBuffers(bool isDynamic);
};
