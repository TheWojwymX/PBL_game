#pragma once

#include "Core/Component.h"
#include <glm/glm.hpp>

class PlayerMovement : public Component {
public:
    PlayerMovement(float speed = 12.0f, float gravity = -9.81f, float jumpHeight = 3.0f, float groundLevel = 0.0f);

    void Input() override;
    void Update() override;

private:
    float _speed;
    float _gravity;
    float _jumpHeight;
    float _groundLevel;
    bool _isGrounded;
    glm::vec3 _velocity;
    glm::vec2 _inputVector;

    void CheckGrounded();
    void HandleMovement();
};
