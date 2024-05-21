#pragma once

#include "Core/Component.h"
#include <glm/glm.hpp>
#include "Component/Camera.h"
#include "Component/BlockManager.h"

class PlayerController : public Component {
public:
    nlohmann::json Serialize() override;
    void Deserialize(const nlohmann::json& jsonData) override;

    PlayerController(float speed = 8.0f, float gravity = -20.0f, float jumpHeight = 3.0f, float groundLevel = 0.5f, float reach = 10.0f, int radius = 2, float width = 0.5f, float height = 1.8f, float digPower = 0.25f);
    void Initiate() override;


    void Init() override;
    void Input() override;
    void Update() override;

    void SetCamera(std::shared_ptr<Camera> cameraRef) { _cameraRef = cameraRef; }
    void SetBlockManager(std::shared_ptr<BlockManager> blockManagerRef) { _blockManagerRef = blockManagerRef; }

    void addToInspector(ImguiMain *imguiMain) override;

    void HandleUpgrades();

private:
    float _speed;
    float _width;
    float _height;
    float _gravity;
    float _jumpHeight;
    float _groundLevel;
    bool _isGrounded;
    bool _jump;
    float _reach;
    int _radius;
    float _digPower;
    glm::vec3 _velocity;
    glm::vec2 _inputVector;
    std::shared_ptr<Camera> _cameraRef;
    int _cameraRefID;
    std::shared_ptr<BlockManager> _blockManagerRef;
    int _blockManagerRefID;
    float _interactionCooldown;
    float _timeSinceLastInteraction;

    void CheckGrounded(glm::vec3 separationVector);
    void HandleMovement();
    void MovementInput();
    void InteractionInput();

    void increaseRadiusLevel(){_radius = _radius + 1;}
    void increaseReachLevel(){_reach = _reach + 1;}
    void increaseSpeedLevel(){_speed = _speed + 1;}
};
