#pragma once

#include "Core/Component.h"
#include <glm/glm.hpp>
#include "Component/Camera.h"
#include "Component/BlockManager.h"
#include "Managers/LightsManager.h"

class ShovelController;

class PlayerController : public Component {
public:
    nlohmann::json Serialize() override;
    void Deserialize(const nlohmann::json& jsonData) override;

    PlayerController(float speed = 8.0f, float gravity = 0.0f, float jumpHeight = 3.0f, float groundLevel = 0.5f, float reach = 10.0f, int radius = 2, float width = 0.5f, float height = 1.8f, float digPower = 0.25f);
    void Initiate() override;


    void Init() override;
    void Input() override;
    void Update() override;

    void SetCamera(std::shared_ptr<Camera> cameraRef) { _cameraRef = cameraRef; }
    void SetBlockManager(std::shared_ptr<BlockManager> blockManagerRef) { _blockManagerRef = blockManagerRef; }

    void addToInspector(ImguiMain *imguiMain) override;

    void HandleUpgrades();

    void UpgradeRadius();

    void UpgradeReach();

    void UpgradeSpeed();

    void SetGravity(float gravity);

    void RestoreFuel() { _jetpackFuel = _maxJetpackFuel; }

    void GetFuelForMaterials();

    float _jetpackFuel = 0.0f;
    float _maxJetpackFuel = 100.0f;

    shared_ptr<ShovelController> _shovelController;

    bool CheckIfPlayerIsAtEntranceToMine();
    bool _activeMineEntranceCollision = false;
    bool _isGrounded;
    float _speed;
    bool _isUsingJetpack = false;

private:
    float _width;
    float _height;
    float _gravity;
    float _jumpHeight;
    float _groundLevel;
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

    int _radiusUpgradeCost = 10;
    int _reachUpgradeCost = 10;
    int _speedUpgradeCost = 10;

    float _jetpackStrength = 35.0f;
    float _jetpackFuelConsumption = 10.0f; // Fuel consumed per second when using the jetpack
    float _jetpackFuelRecovery = 0.0f;     // Fuel recovered per second when not using the jetpack

    bool _spaceKeyWasPressed = false;
    std::chrono::steady_clock::time_point _lastSpacePressTime;
    float _jetpackActivationDelay = 0.25f;

    void JetpackInput();

    bool CheckIsOutsideBase(glm::vec3 playerPos, glm::vec2 domePos, float domeRadius);

    glm::vec3 CircleCollision(glm::vec3 playerPos, glm::vec3 movementVec, glm::vec2 circleCenterPos, float circleRadius, bool isInsideCircle);

};
