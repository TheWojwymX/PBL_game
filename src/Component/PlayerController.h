#pragma once

#include "Core/Component.h"
#include <glm/glm.hpp>
#include "Component/Camera.h"
#include "Component/BlockManager.h"

class PlayerController : public Component {
public:
    PlayerController(float speed = 12.0f, float gravity = -9.81f, float jumpHeight = 3.0f, float groundLevel = 0.5f);

    void Init() override;
    void Input() override;
    void Update() override;

    void SetCamera(std::shared_ptr<Camera> cameraRef) { _cameraRef = cameraRef; }
    void SetBlockManager(std::shared_ptr<BlockManager> blockManagerRef) { _blockManagerRef = blockManagerRef; }

private:
    float _speed;
    float _gravity;
    float _jumpHeight;
    float _groundLevel;
    bool _isGrounded;
    glm::vec3 _velocity;
    glm::vec2 _inputVector;
    std::shared_ptr<Camera> _cameraRef;
    std::shared_ptr<BlockManager> _blockManagerRef;

    void CheckGrounded();
    void HandleMovement();
    void MovementInput();
    void InteractionInput();
};
