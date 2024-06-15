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

    float GetRotation() { return _rotationAngle; }

    void AddRotation(float value);
    void SetRotationAngle(float rotationAngle);

    glm::vec2 CalculateCenter();
    std::pair<glm::vec2, glm::vec2> GetCorners();

    void SetScale(const glm::vec2 &scale);

private:
    glm::vec2 _pixelPosition;
    float _rotationAngle;
    glm::mat4 _model;
    std::array<float, 20> _vertices;
    glm::vec2 _scale = glm::vec2(1.0f, 1.0f);

    float _screenWidth;
    float _screenHeight;

    void UpdateModel();
    void SetupBuffers(bool isDynamic);
};
