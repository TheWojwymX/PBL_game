#pragma once

#include "Core/Component.h"
#include <glm/glm.hpp>
#include "Component/Camera.h"

class PlayerController : public Component {
public:
    PlayerController(float speed = 12.0f, float gravity = -9.81f, float jumpHeight = 3.0f, float groundLevel = 0.0f);

    void Input() override;
    void Update() override;

    void SetCameraRef(std::shared_ptr<Camera> cameraRef);

private:
    float _speed;
    float _gravity;
    float _jumpHeight;
    float _groundLevel;
    bool _isGrounded;
    glm::vec3 _velocity;
    glm::vec2 _inputVector;
    std::shared_ptr<Camera> _cameraRef;

    void CheckGrounded();
    void HandleMovement();
    void MovementInput();
    void InteractionInput();
};
