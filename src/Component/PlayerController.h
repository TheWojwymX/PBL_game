#pragma once

#include "Core/Component.h"
#include <glm/glm.hpp>
#include "Component/Camera.h"
#include "Component/BlockManager.h"

class PlayerController : public Component {
public:
    nlohmann::json Serialize() override;

    PlayerController(float speed = 12.0f, float gravity = -9.81f, float jumpHeight = 3.0f, float groundLevel = 0.5f, float reach = 10.0f, float width = 0.7f, float height = 1.8f);

    void Init() override;
    void Input() override;
    void Update() override;

    void SetCamera(std::shared_ptr<Camera> cameraRef) { _cameraRef = cameraRef; }
    void SetBlockManager(std::shared_ptr<BlockManager> blockManagerRef) { _blockManagerRef = blockManagerRef; }

private:
    float _speed;
    float _width;
    float _height;
    float _gravity;
    float _jumpHeight;
    float _groundLevel;
    bool _isGrounded;
    float _reach;
    glm::vec3 _velocity;
    glm::vec2 _inputVector;
    std::shared_ptr<Camera> _cameraRef;
    std::shared_ptr<BlockManager> _blockManagerRef;

    void CheckGrounded();
    void HandleMovement();
    void MovementInput();
    void InteractionInput();
};
