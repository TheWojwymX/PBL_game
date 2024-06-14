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

    ImageRenderer();

    void Init(const char* file, glm::vec2 initialPosition, float initialRotation, bool isAlpha, bool isDynamic);
    void Init(const char* file, glm::vec2 downLeftCorner, glm::vec2 topRightCorner, bool isAlpha, bool isDynamic);

    bool _shouldRender = true;

    void Render();

    void AddRotation(float value);

    glm::vec2 CalculateCenter();
    std::pair<glm::vec2, glm::vec2> GetCorners();

    void SetRotationAngle(float rotationAngle);

private:
    glm::vec2 _pixelPosition;
    float _rotationAngle;
    glm::mat4 _model;
    std::array<float, 20> _vertices;

    float _screenWidth;
    float _screenHeight;

    void UpdateModel();
    void SetupBuffers(bool isDynamic);
};
