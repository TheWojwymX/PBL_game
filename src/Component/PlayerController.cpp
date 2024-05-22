#include "PlayerController.h"
#include "Core/Time.h"
#include "Core/Input.h"
#include "Core/Node.h"
#include "Managers/GameManager.h"

PlayerController::PlayerController(float speed, float gravity, float jumpHeight, float groundLevel, float reach, int radius, float width, float height, float digPower)
    : _speed(speed), _gravity(gravity), _jumpHeight(jumpHeight), _groundLevel(groundLevel), _isGrounded(false), _velocity(glm::vec3(0.0f)), _inputVector(glm::vec2(0.0f)), _reach(reach), _radius(radius), _width(width), _height(height), _digPower(digPower),
    _interactionCooldown(0.25f), _timeSinceLastInteraction(0.0f) {
    _type = ComponentType::PLAYERCNTROLLER;
}



nlohmann::json PlayerController::Serialize() {
    nlohmann::json data = Component::Serialize();

    if (_blockManagerRef) {
        data["blockManagerRefID"] = _blockManagerRef->_id;
    }

    if (_cameraRef) {
        data["cameraRefID"] = _cameraRef->_id;
    }


    return data;
}

void PlayerController::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("blockManagerRefID")) {
        _blockManagerRefID = jsonData["blockManagerRefID"].get<int>();
    }

    if (jsonData.contains("cameraRefID")) {
        _cameraRefID = jsonData["cameraRefID"].get<int>();
    }

    Component::Deserialize(jsonData);
}

void PlayerController::Initiate() {
    _blockManagerRef = COMPONENTSMANAGER.GetComponentByID<BlockManager>(_blockManagerRefID);
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(_cameraRefID);
    Component::Initiate();
}

void PlayerController::Init() {
    _blockManagerRef->SetCamera(_cameraRef);
}

void PlayerController::Input() {
    MovementInput();
    InteractionInput();
    JetpackInput();
}

void PlayerController::Update() {
    if(!GAMEMANAGER._paused){
        HandleMovement();
        HandleUpgrades();
        //cout << _jetpackFuel << endl;
    }
}

void PlayerController::MovementInput() {
    // Get input for movement along the X and Z axes
    float x = (INPUT.GetKeyDown(GLFW_KEY_D) ? 1.0f : 0.0f) - (INPUT.GetKeyDown(GLFW_KEY_A) ? 1.0f : 0.0f);
    float z = (INPUT.GetKeyDown(GLFW_KEY_W) ? 1.0f : 0.0f) - (INPUT.GetKeyDown(GLFW_KEY_S) ? 1.0f : 0.0f);

    _inputVector = glm::vec2(x, z);
    // Check if input vector is non-zero before normalization
    if (x != 0.0f || z != 0.0f)
        _inputVector = glm::normalize(_inputVector);
}

void PlayerController::InteractionInput() {
    _timeSinceLastInteraction += TIME.GetDeltaTime();

    if (_timeSinceLastInteraction >= _interactionCooldown && INPUT.GetMouseButtonState(GLFW_MOUSE_BUTTON_1)) {
        _blockManagerRef->RayIntersectsBlock(_reach, _radius, _digPower);
        _timeSinceLastInteraction = 0.0f;
    }
}


void PlayerController::CheckGrounded(glm::vec3 separationVector) {
    if (separationVector.y > 0) {
        float roundedY = std::round(_ownerTransform->GetPosition().y + 0.5f) - 0.5f;
        _ownerTransform->SetPosition(roundedY, 1);
        _isGrounded = true;
        _velocity.y = 0.0f;
    }
    else if (separationVector.y < 0) {
        _velocity.y = 0.0f;
    }
    else {
        _isGrounded = false;
    }
}

void PlayerController::HandleMovement() {
    glm::vec3 move = _inputVector.x * _cameraRef->GetRightVector() + _inputVector.y * _cameraRef->GetFrontVector();
    move.y = 0.0f;

    // Only normalize if move vector is not zero
    if (glm::length(move) > 0.0f) {
        if(!_ownerNode->GetComponent<PlayerAudioController>()->_isWalking && _isGrounded){ //are PlayerAudioController steps not working at the moment and walking and on ground
            _ownerNode->GetComponent<PlayerAudioController>()->StartSteps();
        }
        else if(!_isGrounded){ //is walking and not grounded
            _ownerNode->GetComponent<PlayerAudioController>()->StopSteps();
        }
        move = glm::normalize(move) * _speed;
    }
    else{
        if(_ownerNode->GetComponent<PlayerAudioController>()->_isWalking){ //are PlayerAudioController steps working at the moment and there isn't any movement
            _ownerNode->GetComponent<PlayerAudioController>()->StopSteps();
        }
    }

    // Apply gravity
    _velocity.y += _gravity * TIME.GetDeltaTime();

    // Handle jump
    if (_isGrounded && INPUT.IsKeyPressed(GLFW_KEY_SPACE)) {
        _ownerNode->GetComponent<PlayerAudioController>()->PlayJumpSound();
        _ownerNode->GetComponent<ParticleGenerator>()->jumpOffPoint = _ownerTransform->GetPosition();
        _velocity.y = sqrtf(_jumpHeight * -2.0f * _gravity);
        _jump = true;
    }

    // Handle jetpack
    if (_isUsingJetpack && _jetpackFuel > 0) {
        _velocity.y += _jetpackStrength * TIME.GetDeltaTime();
        _jetpackFuel -= _jetpackFuelConsumption * TIME.GetDeltaTime();
        _ownerNode->GetComponent<ParticleGenerator>()->SpawnParticles();
        if (_jetpackFuel < 0) _jetpackFuel = 0;
    } else if (!_isUsingJetpack && _jetpackFuel < _maxJetpackFuel) {
        _jetpackFuel += _jetpackFuelRecovery * TIME.GetDeltaTime();
        if (_jetpackFuel > _maxJetpackFuel) _jetpackFuel = _maxJetpackFuel;
    }

    glm::vec3 movementVector = (move + _velocity) * TIME.GetDeltaTime();

    std::pair<glm::vec3, glm::vec3> collisionResult = _blockManagerRef->CheckEntityCollision(_ownerTransform->GetPosition(), movementVector, _width, _height);

    _ownerTransform->AddPosition(collisionResult.first);

    CheckGrounded(collisionResult.second);
}

void PlayerController::addToInspector(ImguiMain *imguiMain) {
    if (ImGui::TreeNode("Player Controller"))
    {
        // Block Gui related things go there
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();
    }
}

void PlayerController::UpgradeRadius(){
    if (_radius < 10){

        if(GAMEMANAGER._money < _radiusUpgradeCost){
            std::cout << "Nie ma dosc pieniedzy na ulepsznie radiusa" << std::endl;
            return;
        }else{
            GAMEMANAGER._money -= _radiusUpgradeCost;
        }

        increaseRadiusLevel();
        cout << "Radius upgraded" << endl;
    }
    else cout << "MAX radius Level" << endl;
}

void PlayerController::UpgradeReach(){
    if (_reach < 30) {

        if(GAMEMANAGER._money < _reachUpgradeCost){
            std::cout << "Nie ma dosc pieniedzy na ulepsznie reacha" << std::endl;
            return;
        }else{
            GAMEMANAGER._money -= _reachUpgradeCost;
        }

        increaseReachLevel();
        cout << "Reach upgraded" << endl;
    }
    else cout << "MAX reach Level" << endl;
}

void PlayerController::UpgradeSpeed(){
    if (_speed < 15) {

        if(GAMEMANAGER._money < _speedUpgradeCost){
            std::cout << "Nie ma dosc pieniedzy na ulepsznie speeda" << std::endl;
            return;
        }else{
            GAMEMANAGER._money -= _speedUpgradeCost;
        }

        increaseSpeedLevel();
        cout << "Speed upgraded" << endl;
    }
    else cout << "MAX speed Level" << endl;
}

void PlayerController::HandleUpgrades() {
    if(Input::Instance().IsKeyPressed(72)) {
        UpgradeRadius();
    }
    if(Input::Instance().IsKeyPressed(74)) {
        UpgradeReach();
    }
    if(Input::Instance().IsKeyPressed(75)) {
        UpgradeSpeed();
    }
}

void PlayerController::JetpackInput() {
    // Get the current time
    auto now = std::chrono::steady_clock::now();

    // Check if the space key is currently held down
    if (INPUT.GetKeyDown(GLFW_KEY_SPACE)) {
        if (!_spaceKeyWasPressed) {
            // If the space key was not pressed before, update the last press time
            _lastSpacePressTime = now;
            _spaceKeyWasPressed = true;
        }

        // Calculate the elapsed time since the space key was last pressed
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - _lastSpacePressTime).count();

        // Check if the elapsed time is greater than the activation delay and if there's enough fuel
        if (elapsed >= _jetpackActivationDelay && _jetpackFuel > 0) {
            _isUsingJetpack = true;
        } else {
            _isUsingJetpack = false;
        }
    } else {
        // If the space key is not held down, reset the state
        _spaceKeyWasPressed = false;
        _isUsingJetpack = false;
    }
}
